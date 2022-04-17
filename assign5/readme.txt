File: readme.txt
Author: Xiaokun Chen
----------------------

ATM
---
1a)

Defect description: The defect exists because we put unsigned and signed int together to do arithmetic and comparasion so that the signed int would automatically converted to unsigned int. Therefore, if we have a negative int on the left hand side of "<=", that negative int would definitely convert into an unsigned int that is larger than whatever positive(unsigned) int on the left hand side of "<=". Please see the test case for details.
-------------------

Test case explanation (put test case in custom_tests!): I include the test case "samples/atm 108 xiaokunc". The key point is to make the result of left hand side of "<=" negative so that it would convert into a very large number that is way larger than any original unsigned number. The cust->balance is 107 or 0b1101011 and amount is 108 or 0b1101100. The result of (cust->balance - amount) is -1. However, it is considered as a unsigned int, which is 2^32-1 in decimal. It is certainly larger than the MIN_BALANCE, which is 50, if compared as unsigned int.
-------------------------------------------------------

Recommended fix: My recommendation to fix it is to cast the result of (cust->balance - amount) back to signed int so that it would compare the result and the MIN_BALANCE as intended.
----------------

1b)

Defect description: The defect exists because even if we provide a bogus username to the find_account function, it will still return an int even if that is not what it was intended to do. How? Use the eax register. If the username provided does not match any name in the database, the strcmp(username, db->array[i].name) will never return 0, which means it will never enter this if clause. But the eax still remain its value. So, consider the last time in the for loop, after we compare the bogus username and the last name in the database, the result would be returned. And the in the function lookup_by name, find_account get called. Its value was stored in the account_num. If account_num is >= 0 and within the valid range, we could steal money. Please see details in test case explanation.
-------------------

Test case explanation (put test case in custom_tests!): I choose the test case "samples/atm 40 zu". Because the last account's name in the database is called "ztlib". We could just utilize the return value of the strcmp function that compare each char until meet one char that is not equal. Then, use the ASCII of the first argument's char minus the ASCII of the second argument's char. In the test case, 'z' is fisrt considered as equal so we skip it and return 'u' - 't', which is 1. And then we could access the account #1 to withdraw money from it.
-------------------------------------------------------

Recommended fix: Just add one extra return after the end of the for loop: "return NOT_AUTHORIZED;". In this way, using the bogus username to withdraw could be prevented.
----------------

1c)

Defect description: The defect is when we write the path string in the read_secret_passcode function, we could write exceed the range of path and modify the memory space that does not belong to it. Therefore, the memory sapce for code could be changed as the user want it to be. Then, the user could match the known return code and withdraw money from acconut 0!
-------------------

Test case explanation (put test case in custom_tests!): I choose the test case "samples/atm 300 0rtyuiopqwertyuiopi000 3158064". The first 0 in the arg[2] is to tell the program that I want to withdraw money from account 0. Because the strtol function in line 102 would extract the first few numbers from the string and store it in account_num. Then observe that in the assemble code, "code" is store as an int in 0x3c(%rsp), and the path is stored in %rsp. Therefore, we could write exceed the range of the path and change the "code" value. So in the arg[2], I write "0rtyuiopqwertyuiopi" to approach the memory of code(Because the sprintf concatenate BANK_DATA_DIR and filename). Then I overwrite the "code" by three "0", which each has ASCII value of 30, as well as a null terminator, which has ASCII value of 0. Therefore, the whole code would be 0x00303030 because of the reverse order, which corresponds 3158064 in decimal. Therefore I put 3158064 in arg[3] to match the code we get.
-------------------------------------------------------

Recommended fix: The recommendation would be instead of using sizeof(filename), which would be 8 in any cases, we could use the strlen(filename), which would be the actual size for filename string.
----------------

Dataset Aggregation
----

1a)Windom Earle. He/she made several withdrawal of 100000.
1b)Harry Truman. Dale Cooper.
2)I could de-anonymize the data because I check the time people enter the Stanford Bank ATM and the corresponding time in the the transaction.csv. Usually after several minutes they enter the Stanford Bank ATM, they would perform a transaction on the ATM and therefore leaves the record. So I could identify their true name.
3)I would recommend the Stanford Bank to encrypt the real time and date and the amount in the transaction.
4)I think according to the individualist models of privacy, privacy is essentially personal and privacy is an individual right. Therefore, the information that can be gathered here is a violation of privacy because we invade the "personal" attribute of privacy.

SecureVault
----
1)I use the breakpoint at trigger_alarm so that whenever I am about to trigger the alarm. Also, I use nexti to step over each line, whenever I find it jump to the line of triggering the alarm, I quit.
2)This instruction is to move the address of the string to the $edi.
This hex value is the address of a string, which is the password for the level_1. It is moved to $edi because we are making preparation for calling the strcmp function, which compares the input line and the correct password.
Because it is just the address of the string, we don't have to use $rdi.
3)If not immediately preceded by a cmp or test instruction, jp would check if ZF=0 and SF=OF. If so, jg would jump. In our case, we know that xor would set the OF = 0. And the ZF is 0 according to the previous test. So that if the SF=OF=0, then we would jump.
4)The loop in winky first git rid of the first character of the input string. In my case, it is the second argument of line 3, which is "sspoiling". So the first character that is taken out is "s". Then compare the rest with each word in the word list, if they are the same, return the "value". The "value" is call strspn on each word in the word lis with the first character, which is in form "strspn(eachword, "s")", then aggregate each value everytime. So, in the last round, if I already satify the value of the agregation, I just need to provide the next word to match the next world in strcmp. So, in total, my string is "sspoiling".  
5)The mycmp compares two hex integer, the order is put them separately into the qghrjd, which returns the value itself if it is less or equal to 0xf, or right shift 4 bits until less or equal to 0xf. Then compare the results after the qghrid. The type of data compared is the integer(4 bits each).
6)During this process, the bank headquarter and branches are trusted. Because on the differential privacy's trust model, we rely on, or trust the bank headquarter to manage and keep our data safe. The threat of our case is that some user accesss the data that are not permitted. Also, I am trusted by the bank to do the test on the new system.
