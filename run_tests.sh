#!/bin/bash
# MFMS automated black-box tests (group version).
# Run from the project folder:  bash tests/run_tests.sh      (or: make test)
# Each test types keyboard input into the program and checks the text that appears.
# "BUG" tests check behaviour that is currently WRONG in a module; they are counted
# separately so the bug list can be reported to the module owner (GitHub Issue).

cd "$(dirname "$0")/.." || exit 1
gcc -std=c99 -o mfms main.c common.c employees.c budget.c suppliers.c assets.c reports.c || { echo "Build failed"; exit 1; }

PASS=0; FAIL=0; BUGS=0; FIXED=0

# check_output "input" "args" expected...   -> returns 0 when all expected texts appear
check_output() {
    local input="$1" args="$2"; shift 2
    local output
    output=$(printf "%b" "$input" | timeout 5 ./mfms $args 2>&1 | head -c 100000)
    for expected in "$@"; do
        grep -qF -- "$expected" <<<"$output" || { MISSING="$expected"; return 1; }
    done
    return 0
}

# run_test "ID description" "args" "input" expected...
run_test() {
    local name="$1" args="$2" input="$3"; shift 3
    if check_output "$input" "$args" "$@"; then
        echo "PASS  $name"; PASS=$((PASS+1))
    else
        echo "FAIL  $name   (missing: \"$MISSING\")"; FAIL=$((FAIL+1))
    fi
}

# bug_test: expected behaviour that the module does not yet have
bug_test() {
    local name="$1" args="$2" input="$3"; shift 3
    if check_output "$input" "$args" "$@"; then
        echo "FIXED $name"; FIXED=$((FIXED+1))
    else
        echo "BUG   $name"; BUGS=$((BUGS+1))
    fi
}

echo "=== MFMS TEST RUN ==="
echo "--- Main menu ---"
run_test "T01 Main menu shows all options"        ""       "6\n" "Employee Management" "Budget Management" "Supplier Management" "Asset Management" "Reports" "Goodbye"
run_test "T02 Main menu: letters rejected"        ""       "abc\n6\n" "Invalid input. Please enter a number from 1 to 6."
run_test "T03 Main menu: out-of-range rejected"   ""       "9\n0\n6\n" "Invalid choice. Please select 1 to 6."
run_test "T04 Demo mode loads sample data"        "--demo" "6\n" "Demo data loaded."

echo "--- Employee management ---"
run_test "T05 Add employee (valid)"               ""       "1\n1\n10\nAnna Smith\nFinance\n10000\n2000\n1000\n0\n5\n6\n" "Employee added successfully" "FIN-0010"
run_test "T06 Negative basic salary rejected"     ""       "1\n1\n10\nAnna Smith\nFinance\n-500\n10000\n0\n0\n0\n5\n6\n" "Value cannot be less than 0.01"
run_test "T07 Empty name rejected"                ""       "1\n1\n10\n\nAnna Smith\nFinance\n10000\n0\n0\n0\n5\n6\n" "This field cannot be empty"
run_test "T08 Name with digits rejected"          ""       "1\n1\n10\nAnna123\nAnna Smith\nFinance\n10000\n0\n0\n0\n5\n6\n" "Names may only contain letters"
run_test "T09 Duplicate employee ID rejected"     "--demo" "1\n1\n101\n5\n6\n" "already exists"
run_test "T10 Salary calculation correct"         ""       "1\n1\n10\nAnna Smith\nFinance\n10000\n2000\n1000\n0\n4\n10\n5\n6\n" "Gross salary      : N\$13000.00" "Income tax        : N\$1125.00" "NET SALARY        : N\$11375.00"
run_test "T11 Search employee by ID"              "--demo" "1\n3\n1\n105\n5\n6\n" "Ndapewa Amupolo"
run_test "T12 Search employee by partial name"    "--demo" "1\n3\n2\nshik\n5\n6\n" "Maria Shikongo"
run_test "T13 Search employee: not found"         "--demo" "1\n3\n1\n999\n5\n6\n" "No employee found with ID 999"

echo "--- Budget management (group module) ---"
run_test "T14 Add budget within limit"            ""       "2\n1\nFinance\n500000\n420000\n5\n6\n" "Remaining Budget: N\$80000.00" "WITHIN BUDGET"
run_test "T15 Add budget over limit"              ""       "2\n1\nWorks\n1000\n1500\n5\n6\n" "EXCEEDED BUDGET"
run_test "T16 Negative budget rejected"           ""       "2\n1\nFinance\n-100\n5\n6\n" "Negative budget not allowed"
run_test "T17 Negative expenditure rejected"      ""       "2\n1\nFinance\n1000\n-5\n5\n6\n" "Negative expenditure not allowed"
run_test "T18 Budget menu: invalid number"        ""       "2\n9\n5\n6\n" "Invalid choice! Try again."
run_test "T19 Display budgets (demo)"             "--demo" "2\n2\n5\n6\n" "Department: Finance" "Remaining Budget: N\$80000.00"
run_test "T20 Search budget: found"               "--demo" "2\n3\nHealth\n5\n6\n" "Found!" "Allocated: N\$800000.00"
run_test "T21 Search budget: not found"           "--demo" "2\n3\nNoSuchDept\n5\n6\n" "not found"
run_test "T22 Departments exceeding budget"       "--demo" "2\n4\n5\n6\n" "Public Works : Over by N\$150000.00"
run_test "T23 Exceeded list when none exceed"     ""       "2\n1\nFinance\n500000\n420000\n4\n5\n6\n" "All departments are WITHIN BUDGET"

echo "--- Supplier management (group module) ---"
run_test "T24 Add supplier (valid)"               ""       "3\n1\nS9\nABC Traders\nabc@traders.com.na\n0611234567\nWindhoek\n4\n6\n" "Supplier added successfully"
run_test "T25 Invalid email rejected"             ""       "3\n1\nS9\nABC Traders\nnot-an-email\nabc@traders.com.na\n0611234567\nWindhoek\n4\n6\n" "Invalid email"
run_test "T26 Invalid phone rejected"             ""       "3\n1\nS9\nABC Traders\nabc@traders.com.na\nabc123\n0611234567\nWindhoek\n4\n6\n" "Invalid telephone number"
run_test "T27 Duplicate supplier ID rejected"     "--demo" "3\n1\nS1\n4\n6\n" "Supplier ID already exists"
run_test "T28 Display suppliers (demo)"           "--demo" "3\n2\n4\n6\n" "Windhoek Office Supplies" "Total Suppliers: 4"
run_test "T29 Search supplier by ID"              "--demo" "3\n3\n1\nS2\n4\n6\n" "Supplier found!" "Coastal Hardware"
run_test "T30 Search supplier by name"            "--demo" "3\n3\n2\nCapital Fuel Traders\n4\n6\n" "Supplier found!" "S4"
run_test "T31 Search supplier: not found"         "--demo" "3\n3\n1\nS99\n4\n6\n" "Supplier not found"
run_test "T32 Supplier menu: invalid number"      ""       "3\n9\n4\n6\n" "Invalid choice. Please select 1 to 4."

echo "--- Asset management (group module) ---"
run_test "T33 Add asset (valid)"                  ""       "4\n1\nA010\nLaser Printer\n2\n5500\nFinance\n1\n5\n6\n" "added successfully"
run_test "T34 Negative asset value rejected"      ""       "4\n1\nA010\nPrinter\n2\n-5\n5500\nFinance\n1\n5\n6\n" "enter a number greater than 0"
run_test "T35 Asset: letters as value rejected"   ""       "4\n1\nA010\nPrinter\n2\nabc\n5500\nFinance\n1\n5\n6\n" "enter a number greater than 0"
run_test "T36 Asset: empty name rejected"         ""       "4\n1\nA010\n\nPrinter\n2\n5500\nFinance\n1\n5\n6\n" "this field cannot be empty"
run_test "T37 Asset: invalid type choice"         ""       "4\n1\nA010\nPrinter\n9\n2\n5500\nFinance\n1\n5\n6\n" "invalid choice, enter a number from 1 to 6"
run_test "T38 Duplicate asset ID rejected"        "--demo" "4\n1\nA001\nA010\nPrinter\n2\n100\nFinance\n1\n5\n6\n" "already exists"
run_test "T39 Asset menu: invalid number"         ""       "4\n9\n5\n6\n" "Invalid choice! Enter a number from 1 to 5."
run_test "T40 Display asset register (demo)"      "--demo" "4\n2\n5\n6\n" "ASSET REGISTER (5)" "Town Hall Building"
run_test "T41 Search asset by ID"                 "--demo" "4\n3\n1\nA004\n5\n6\n" "Asset found:" "Water Pump Unit"
run_test "T42 Search asset by type"               "--demo" "4\n3\n2\n1\n5\n6\n" "Toyota Hilux"
run_test "T43 Search asset by department"         "--demo" "4\n3\n3\nIT\n5\n6\n" "Dell Laptop"
run_test "T44 Search asset: not found"            "--demo" "4\n3\n1\nA999\n5\n6\n" "not found"

echo "--- Reports ---"
run_test "T45 Employee report values"             "--demo" "5\n1\n6\n6\n" "Total Employees : 6" "Highest Salary  : N\$33000.00 (Ndapewa Amupolo)" "Lowest Salary   : N\$8300.00 (Tangeni Iipinge)"
run_test "T46 Budget report totals"               "--demo" "5\n2\n6\n6\n" "Total Allocated Budget : N\$2800000.00" "Total Expenditure      : N\$2705000.00" "Public Works"
run_test "T47 Supplier report"                    "--demo" "5\n3\n6\n6\n" "SUPPLIER REPORT" "Total Suppliers: 4"
run_test "T48 Asset report (from reports menu)"   "--demo" "5\n4\n6\n6\n" "ASSET REPORT" "Poor = 1" "Most Valuable Asset: Town Hall Building"
run_test "T49 Asset report (from asset menu)"     "--demo" "4\n4\n5\n6\n" "Total Assets: 5"
run_test "T50 All reports with no data (no crash)" ""      "5\n5\n6\n6\n" "No employees registered yet" "No budgets entered yet" "No suppliers registered" "No assets registered yet"
run_test "T51 Budget entered then reported"       ""       "2\n1\nFinance\n500000\n420000\n5\n5\n2\n6\n6\n" "Total Allocated Budget : N\$500000.00" "None - all departments are within budget"
run_test "T52 Supplier entered then reported"     ""       "3\n1\nS9\nABC Traders\nabc@traders.com.na\n0611234567\nWindhoek\n4\n5\n3\n6\n6\n" "ABC Traders" "Total Suppliers: 1"

echo "--- Robustness / known bugs in group modules ---"
run_test "T53 End of input at main menu exits"    ""       "" "Invalid input"
bug_test "BUG1 Budget menu: letters should be rejected (program hangs)"   "" "2\nabc\n5\n6\n" "Invalid" "Goodbye"
bug_test "BUG2 Budget: letters at amount prompt (program hangs)"          "" "2\n1\nFinance\nabc\n5\n6\n" "Invalid" "Goodbye"
bug_test "BUG3 Budget: duplicate department should be rejected"           "--demo" "2\n1\nFinance\n1000\n10\n5\n6\n" "already exists"

echo "======================"
echo "Passed: $PASS   Failed: $FAIL   Known bugs: $BUGS   Bugs fixed: $FIXED"
[ $FAIL -eq 0 ]
