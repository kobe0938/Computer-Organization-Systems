#!/usr/bin/python

from testing import *
import results

class CountLines(BaseTest):

    nlevels = 4

    def score(self, student_ex, context):
        nlines = int(student_ex.output)
        return results.Points(score=(nlines >= self.nlevels),
            short="Counted %d lines in your input.txt file, expected %d" % (nlines, self.nlevels))

class ATM(BaseTest):

    success_expected = True

    def score(self, student_ex, context):
        success_output = util.match_regex("\[Transaction Log.*", student_ex.output)
        fail_output = util.match_regex("Request failed.*", student_ex.output)
        cash_dispersed = '$$' in student_ex.output
        achieved = student_ex.exitcode == 0 and success_output and cash_dispersed
        out = success_output or fail_output or student_ex.output
        msg = "Expected %s got %s"% ("success" if self.success_expected else "reject", out)
        if self.success_expected and achieved:
            return results.Correct(short=msg)
        elif not self.success_expected and not achieved:
            return results.Correct(short=msg)
        else:
            return results.Incorrect(short=msg)


class CustomATM(ATM):

    is_custom_template = True
    success_expected = True
    soln_irrelevant = True

    def init_from_string(self, line, num):
        self.name = "Custom-%d" % num
        command_str = line.strip()  # remove leading/trailing whitespace
        exec_name = command_str.split()[0]  # break off first token
        assert(exec_name == "samples/atm"), "Custom command must start with samples/atm, not %s" % (exec_name)
        self.command = "$core_cmd " + command_str
        return self

    def execute_solution(self, wd):
        return "OK"

