import sys
import argparse
import psutil # This gets imported into our environment via setup script
import subprocess
import os
from typing import Dict, List, Tuple
import time
import random
import shutil

# Copy the scripts folder over to the build folder, before we run... only if the scripts were copied over before..
if os.path.exists(os.path.abspath(os.path.join(__file__, "../../.Build/Bin/scripts"))):
    shutil.rmtree(os.path.abspath(os.path.join(__file__, "../../.Build/Bin/scripts"))) # cleanup old folder
    shutil.copytree(os.path.abspath(os.path.join(__file__, "../../scripts")), os.path.abspath(os.path.join(__file__, "../../.Build/Bin/scripts")))

def timeInMs() -> int:
    return round(time.time() * 1000)
def simplifyBytes(_input: int) -> str:
    if _input / pow(1024, 3) > 1.0:                             # GBs
        return str('%.2f' % (_input / pow(1024, 3))) + "GBs"
    if _input / pow(1024, 2) > 1.0:                             # MBs
        return str('%.2f' % (_input / pow(1024, 2))) + "MBs"
    if _input / 1024 > 1.0:                                     # KBs
        return str('%.2f' % (_input / 1024)) + "KBs"
    return str('%.2f' % _input) + "Bs"                          # Bytes?

# Thanks: https://stackoverflow.com/a/13607392
class ProcessTimer:
    def __init__(self, command, stdout: int = subprocess.STDOUT, cwd:str = None, shell: bool = (sys.platform != 'win32')):
        self.command = command
        self.execution_state = False
        self.stdout = stdout
        self.shell = shell
        self.cwd = cwd

    def execute(self):
        self.max_vms_memory = 0
        self.max_rss_memory = 0

        self.t1 = None
        self.t0 = timeInMs()
        self.p = subprocess.Popen(self.command, shell=self.shell, stdout=self.stdout, cwd=self.cwd)
        self.execution_state = True

    def poll(self):
        if not self.check_execution_state():
            return False
        self.t1 = timeInMs()
        try:
            pp = psutil.Process(self.p.pid)

            #obtain a list of the subprocess and all its descendants
            descendants = list(pp.children(recursive=True))
            descendants = descendants + [pp]

            rss_memory = 0
            vms_memory = 0

            #calculate and sum up the memory of the subprocess and all its descendants 
            for descendant in descendants:
                try:
                    mem_info = descendant.memory_full_info()

                    rss_memory += mem_info.rss
                    vms_memory += mem_info.vms
                except Exception:
                    #sometimes a subprocess descendant will have terminated between the time
                    # we obtain a list of descendants, and the time we actually poll this
                    # descendant's memory usage.
                    pass
            self.max_vms_memory = max(self.max_vms_memory,vms_memory)
            self.max_rss_memory = max(self.max_rss_memory,rss_memory)
        except psutil.NoSuchProcess:
            return self.check_execution_state()
        return self.check_execution_state()

    def is_running(self):
        return psutil.pid_exists(self.p.pid) and self.p.poll() == None

    def check_execution_state(self):
        if not self.execution_state:
            return False
        if self.is_running():
            return True
        self.executation_state = False
        self.t1 = timeInMs()
        return False

    def close(self,kill=False):
        try:
            pp = psutil.Process(self.p.pid)
            if kill:
                pp.kill()
            else:
                pp.terminate()
        except psutil.Error:
            pass

def exec(_target: str, _args: List[str]) -> Tuple[int, int, int, str]:
    execution: list = [_target]
    proc: ProcessTimer = ProcessTimer(" ".join(execution + _args), stdout=subprocess.PIPE, cwd=os.path.abspath(os.path.join(__file__, "../../.Build/Bin")))
    try:
        proc.execute()
        while proc.poll() or proc.is_running():
            pass # Wait until we are done...
    finally:
        proc.close()
    output = proc.p.communicate()[0].decode('utf-8').replace('\r', '').replace('\n', '')
    maxUsage = proc.max_rss_memory
    noOpTime = 10
    elapsed = proc.t1 - (proc.t0 + noOpTime)
    rt = proc.p.returncode

    if rt != 0:
        print(" ".join(execution + _args))

    return (maxUsage, elapsed, rt, output)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("targetPath", type=str)
    parser.add_argument("-entries", type=int)
    parser.add_argument("-iterations", type=int)
    parser.add_argument("--show-values", action="store_true")
    args = parser.parse_args()
    TARGET_PATH             : str = os.path.abspath(args.targetPath)
    TARGET_ENTRIES          : int = args.entries or 50
    TARGET_SHOW_VALUES      : int = args.show_values
    TARGET_AVERAGE_ITERATION: int = args.iterations or 10

    # {testName: {language : <time, memUsage>}}
    readmeTable: Dict[str, Dict[str, Tuple[int, int]]] = {}

    languages: List[str] = [
        "Cpp",
        "Csharp",
        "Lua",
        "LuaJIT",
        # "Java",
        "Javascript",
        # "Julia",
        "Python",
    ]
    languages.sort()
    search_tests: Dict[str, str] = {
        "[Array] Linear Search"             :   "Array_Search_Linear",
        "[Array] Binary Search"             :   "Array_Search_Binary",
        "[Array] Jump Search"               :   "Array_Search_Jump",
        "[Array] Interpolation Search"      :   "Array_Interpolation_Search",
        "[Array] Exponential Search"        :   "Array_Exponential_Search"
    }
    sorting_tests: Dict[str, str] = {
        "[Array] Selection Sort"        :   "Array_selectionSort",
        "[Array] Bubble Sort"           :   "Array_bubbleSort",
        "[Array] Insertion Sort"        :   "Array_insertionSort",
        # "[Array] Counting Sort"         :   "Array_countSort", # Not ready...
        "[Array] Quick Sort"            :   "Array_quickSort",
        "[Array] Radix Sort"            :   "Array_radixSort"
    }
    language_rules: Dict[str, Dict[str, str]] = {
        'Cpp': {'[': '', ']': ''},
        'Csharp': {'[': '', ']': ''},
        'Lua': {'[': '{', ']': '}'},
        'LuaJIT': {'[': '{', ']': '}'},
    }

    testArray: list[int] = []
    for i in range(0, TARGET_ENTRIES):
        testArray.append(str(random.randrange(0, 9999)))
    testExpectation = random.randrange(0, TARGET_ENTRIES)

    for lang in languages:
        TARGET_EXECUTABLE = os.path.abspath(os.path.join(TARGET_PATH, "Test-" + lang + (".exe" if 'win' in sys.platform else '')))
        if not os.path.exists(TARGET_EXECUTABLE):
            print("Could not find target executable:", TARGET_EXECUTABLE)
            continue

        for testName in search_tests.keys():
            avg_maxUsage = 0
            avg_executionTime = 0
            passed = None
            for iteration in range(0, TARGET_AVERAGE_ITERATION):
                # we need to give sorted arrays here.
                sortedTestArray: List[int] = [int(x) for x in testArray]
                sortedTestArray.sort() # Ensure that the test array is sorted (Not on Measurement).
                sortedTestArrayStr: List[str] = [str(x) for x in sortedTestArray]
                testCase = sortedTestArrayStr[testExpectation]
                maxUsage, executionTime, returnCode, stdoutput = exec(TARGET_EXECUTABLE, [
                        search_tests.get(testName),
                        language_rules.get(lang, {}).get('[', '[') + ",".join(sortedTestArrayStr) + language_rules.get(lang, {}).get(']', ']'),
                        str(testCase)
                    ])
                value = None
                try:
                    value = int(stdoutput) if returnCode == 0 else None
                except:
                    pass
                if passed == None or passed == True:
                    if returnCode == 0:
                        if value == testExpectation:
                            passed = True
                avg_maxUsage += maxUsage
                avg_executionTime += executionTime
            avg_maxUsage /= TARGET_AVERAGE_ITERATION
            avg_executionTime /= TARGET_AVERAGE_ITERATION

            if TARGET_SHOW_VALUES:
                print('✅' if returnCode == 0 and passed else '❌', returnCode, lang, testName, str(avg_executionTime) + "ms", simplifyBytes(avg_maxUsage), testExpectation, '=?', value)
            else:
                print('✅' if returnCode == 0 and passed else '❌', returnCode, lang, testName, str(avg_executionTime) + "ms", simplifyBytes(avg_maxUsage))

            # Logging Data
            if readmeTable.get(testName, None) is None:
                readmeTable[testName] = {}
            if readmeTable[testName].get(lang, None) is None:
                readmeTable[testName][lang] = (avg_executionTime, avg_maxUsage)

        for testName in sorting_tests.keys():
            avg_maxUsage = 0
            avg_executionTime = 0
            testCase = testArray[testExpectation]
            passed = None
            for iteration in range(0, TARGET_AVERAGE_ITERATION):
                maxUsage, executionTime, returnCode, value = exec(TARGET_EXECUTABLE, [
                        sorting_tests.get(testName),
                        language_rules.get(lang, {}).get('[', '[') + ",".join(testArray) + language_rules.get(lang, {}).get(']', ']')
                        ])
                if passed == None or passed == True:
                    if returnCode == 0:
                        sortedTestArray: List[int] = [int(x) for x in testArray]
                        sortedTestArray.sort() # Ensure that the test array is sorted (Not on Measurement).
                        sortedTestArrayStr: List[str] = [str(x) for x in sortedTestArray]
                        answerArrayStr: List[str] = []
                        for x in value.split(','):
                            try:
                                if int(x):
                                    answerArrayStr.append(x)
                            except:
                                pass
                        # print(testName, "\nHost:", ",".join(answerArrayStr), "\nLocal:", ",".join(sortedTestArrayStr))
                        if ",".join(answerArrayStr) == ",".join(sortedTestArrayStr):
                            passed = True
                avg_maxUsage += maxUsage
                avg_executionTime += executionTime
            avg_maxUsage /= TARGET_AVERAGE_ITERATION
            avg_executionTime /= TARGET_AVERAGE_ITERATION

            if TARGET_SHOW_VALUES:
                print('✅' if returnCode == 0 and passed else '❌', returnCode, lang, testName, str(avg_executionTime) + "ms", simplifyBytes(avg_maxUsage), ",".join(sortedTestArrayStr), '=?', ",".join(answerArrayStr))
            else:
                print('✅' if returnCode == 0 and passed else '❌', returnCode, lang, testName, str(avg_executionTime) + "ms", simplifyBytes(avg_maxUsage))

            # Logging Data
            if readmeTable.get(testName, None) is None:
                readmeTable[testName] = {}
            if readmeTable[testName].get(lang, None) is None:
                readmeTable[testName][lang] = (avg_executionTime, avg_maxUsage)

    outputTable: str = "| \t\t\t"
    # Language list [Header]
    for language in languages:
        outputTable += "| " + language + "\t"
    outputTable += "|\n"
    # Break line
    for language in languages:
        outputTable += "| " + "---" + "\t\t"
    outputTable += "| ---\t\t|\n"
    # Body
    for testName in readmeTable.keys():
        outputTable += testName
        for language in readmeTable[testName].keys():
            avg_execTime, avg_memUsage = readmeTable[testName][language]
            outputTable +=  " | " + str(avg_execTime) + "ms" + "/" + simplifyBytes(avg_memUsage)
        outputTable += "\n"
    print(outputTable)
if __name__ == '__main__':
    main()
