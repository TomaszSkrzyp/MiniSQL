import subprocess
import os
import time

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
# Absolute or relative path to the newly built executable
EXE_PATH = r"C:\Things\MiniSQL\out\build\Visual Studio Community 2022 Release - amd64\Debug\minisql_exe.exe"
DATA_FILE = os.path.join(BASE_DIR, "MiniSQL", "data.bin")

def send_cmd(process, cmd):
    
    process.stdin.write(cmd + "\n")
    process.stdin.flush()
    
    output = ""
    while True:
        char = process.stdout.read(1)
        if not char:
            break
        output += char
        if output.endswith("miniSQL > "):
            break
            
    clean_output = output.replace("miniSQL > ", "").strip()
        
    return clean_output

def run_test():
    if os.path.exists(DATA_FILE):
        os.remove(DATA_FILE)
        print(f"Removed {DATA_FILE}")

    process = subprocess.Popen(
        [EXE_PATH],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=0
    )

    try:
        print("--- Starting Tests ---")
        startup_buffer = ""
        while not startup_buffer.endswith("miniSQL > "):
            char = process.stdout.read(1)
            startup_buffer += char
        print(f"[DEBUG] System ready. Caught startup prompt.")

        res = send_cmd(process, "INSERT 1 Alice")
        print(f"[RESULT] INSERT: '{res}'")
        assert "Inserted." in res

        res = send_cmd(process, "SELECT ALL")
        print(f"[RESULT] SELECT ALL: '{res}'")
        assert "1 | Alice" in res

        res = send_cmd(process, "SELECT WHERE NAME=Alice")
        print(f"[RESULT] SELECT WHERE NAME: {res}")
        assert "1 | Alice" in res

        res = send_cmd(process, "SELECT WHERE ID=1")
        print(f"[RESULT] SELECT WHERE ID: {res}")
        assert "1 | Alice" in res

        res = send_cmd(process, "INSERT 1 Bob")
        print(f"[RESULT] DUPLICATE: '{res}'")
        assert "Error: Duplicate ID." in res

        res = send_cmd(process, "UPDATE 1 Frank")
        print(f"[RESULT] UPDATE: '{res}'")
        assert "Updated." in res

        res = send_cmd(process, "SELECT ALL")
        print(f"[RESULT] SELECT ALL: '{res}'")
        assert "1 | Frank" in res

        res = send_cmd(process, "DELETE 1")
        print(f"[RESULT] DELETE: '{res}'")
        assert "Deleted." in res

        res = send_cmd(process, "SELECT ALL")
        print(f"[RESULT] SELECT ALL (Empty check): '{res}'")
        assert "1 | Frank" not in res

        print("\n--- All Tests Passed! ---")

    except AssertionError:
        print("\n[!] Test Failed!")
    finally:
        process.terminate()

if __name__ == "__main__":
    run_test()