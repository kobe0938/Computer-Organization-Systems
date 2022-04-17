File: readme.txt
Assignment: assign0
Author: Xiaokun Chen
Pronouns: he/him
----------------------

0. Tell us about yourself!  What do you do for fun?  Tell a quick anecdote about something that you feel makes you unique; a talent, experience, anything.

Hi everyone! I'm Xiaokun Chen from Nanjing, China. I love playing hearthstone and jet ski. I feel so excited to take this course at Stanford :)

1. Please initial below to confirm your course enrollment.

[y] I will attend the CS107 final as scheduled and I understand no alternate/makeup final exams will be offered except for OAE or official athletics accommodations. The final exam is Thursday, December 9th, 12:15-3:15PM.
[y] I have completed the Honor Code quiz on the assign0 page, and I have read and understood the CS107 collaboration policy at https://cs107.stanford.edu/collaboration.html. By adding my initials, I affirm my intention to abide by this policy.

INTRUDER DETECTION ACTIVITY
2a)I observed that the user.list is a file itself and contains the authorized users and the users inside the home directory may have a difference to the user.list. That difference is the intruder. I used these commands: ls; cd; ls -R > ~/cs107/assign0/list.txt;(this list all things in the current dir and write them into list.txt);  diff users.list ~/cs107/assign0/list.txt(this compare those two lists. It turns out that intruder is mattv!)

2b)The hidden activity file is called .bash_history. It contains all the command lines the intruder executed. I found the file by using the following commands: cd; ls; ls -a;(this one list all the files in the dir, including the hidden one); cat .bash_history(this one print the command history)

2c)The sudo commands are:
sudo dmesg
sudo emacs /etc/passwd
sudo cp -r /etc /mnt/usb/etc
sudo su
Like the 2b), first go to the mattv dir. Then search "sudo" inside the .bash_history file. I found them by using these Unix commands: cd; ls; ls -a; grep "sudo" .bash_history(this command search the "sudo" inside the file .bash_history.)
