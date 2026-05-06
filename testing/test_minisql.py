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
        print(f"[DEBUG] System ready.\n")

        print("--- Standard Functional Tests ---")
        
        res = send_cmd(process, "INSERT INTO (id, name) VALUES (1, \"Alice\")")
        print(f"[RESULT] INSERT: '{res}'")
        assert "Inserted." in res

        res = send_cmd(process, "SELECT *")
        print(f"[RESULT] SELECT *: '{res}'")
        assert "1 | Alice" in res

        res = send_cmd(process, "SELECT WHERE ID=1")
        print(f"[RESULT] SELECT WHERE ID: '{res}'")
        assert "1 | Alice" in res

        res = send_cmd(process, "UPDATE SET NAME = 'Frank' WHERE ID = 1")
        print(f"[RESULT] UPDATE: '{res}'")
        assert "Updated." in res

        print("\n--- Syntax & Chaos Tests ---")

        res = send_cmd(process, "   sElEcT    *    ")
        print(f"[RESULT] Case & Spacing: '{res}'")
        assert "1 | Frank" in res

        res = send_cmd(process, "SELECT WHERE ID = Alice")
        print(f"[RESULT] Type Mismatch: '{res}'")
        assert "Invalid query." in res

        res = send_cmd(process, 'INSERT INTO (id, name) (2, "Broken")')
        print(f"[RESULT] Missing VALUES: '{res}'")
        assert "Invalid query." in res

        res = send_cmd(process, "UPDATE SET NAME = 'Hacker'")
        print(f"[RESULT] Missing WHERE: '{res}'")
        assert "Invalid query." in res

        res = send_cmd(process, 'INSERT INTO (id, name) VALUES (42, "Name; WITH; symbols!")')
        print(f"[RESULT] String literals: '{res}'")
        assert "Inserted." in res

        res = send_cmd(process, "GIVE ME ALL DATA")
        print(f"[RESULT] Garbage Input: '{res}'")
        assert "Invalid query." in res

        print("\n--- Boundary & Cleanup Tests ---")

        res = send_cmd(process, 'INSERT INTO (id, name) VALUES (9999, "Last")')
        assert "Inserted." in res

        res = send_cmd(process, "DELETE FROM WHERE ID = 1")
        print(f"[RESULT] DELETE ID 1: '{res}'")
        assert "Deleted." in res

        res = send_cmd(process, "DELETE FROM WHERE ID = 42")
        print(f"[RESULT] DELETE ID 42: '{res}'")
        assert "Deleted." in res
        res = send_cmd(process, "DELETE FROM WHERE ID = 9999")
        print(f"[RESULT] DELETE ID 9999: '{res}'")
        assert "Deleted." in res

        res = send_cmd(process, "SELECT *")
        print(f"[RESULT] Final SELECT * (Empty): '{res}'")
        assert "1 | Frank" not in res
        assert "42 | Name; WITH; symbols!" not in res
        assert "9999 | Last" not in res

        print("\n--- All Tests Passed! ---")

    except AssertionError:
        print("\n[!] Test Failed!")
    except Exception as e:
        print(f"\n[!] Unexpected Error: {e}")
    finally:
        process.terminate()

if __name__ == "__main__":
    run_test()