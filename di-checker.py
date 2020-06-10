#!/usr/bin/env python
#
# DI Checker summary.
#

from __future__ import print_function
import argparse
import os
import sys
from json import loads
from collections import defaultdict
from collections import OrderedDict

class DILocBug:
  def __init__(self, action, bb_name, fn_name, instr):
    self.action = action
    self.bb_name = bb_name
    self.fn_name = fn_name
    self.instr = instr

class DISPBug:
  def __init__(self, action, fn_name):
    self.action = action
    self.fn_name = fn_name

# Report the bugs in form of html.
def generate_html_report(di_location_bugs, di_subprogram_bugs, \
                         di_location_bugs_summary, di_sp_bugs_summary, \
                         html_file):
  fileout = open(html_file, "w")

  html_header = "<html>\n"
  html_header += "<head>\n"
  html_header += "<style>\n"
  html_header += "table, th, td {\n"
  html_header += "  border: 1px solid black;\n"
  html_header += "}\n"
  html_header += "table.center {\n"
  html_header += "  margin-left: auto;\n"
  html_header += "  margin-right: auto;\n"
  html_header += "}\n"
  html_header += "</style>\n"
  html_header += "</head>\n"
  html_header += "<body>\n"

  # Create the table for Location bugs.
  table_di_loc = "<table>\n"

  # Create the table's column headers.
  table_title_di_loc = "Location Bugs found by the LLVM DI Checker"
  table_di_loc += "<caption><b>"
  table_di_loc += table_title_di_loc
  table_di_loc += "</b></caption>\n"

  header_di_loc = ["File", "LLVM Pass Name", "LLVM IR Instruction", "Function Name", "Basic Block Name", "Action"]
  table_di_loc += "  <tr>\n"

  for column in header_di_loc:
    table_di_loc += "    <th>{0}</th>\n".format(column.strip())
  table_di_loc += "  </tr>\n"

  at_least_one_bug_found = False

  # Handle loction bugs.
  for file, per_file_bugs in di_location_bugs.items():
    for llvm_pass, per_pass_bugs in per_file_bugs.items():
      # No location bugs for the pass.
      if len(per_pass_bugs) == 0:
        continue
      at_least_one_bug_found = True
      row = []
      table_di_loc += "  </tr>\n"
      # Get the bugs info.
      for x in per_pass_bugs:
        row.append("    <tr>\n")
        row.append(file)
        row.append(llvm_pass)
        row.append(x.instr)
        row.append(x.fn_name)
        row.append(x.bb_name)
        row.append(x.action)
        row.append("    </tr>\n")
      # Dump the bugs info into the table.
      for column in row:
        # The same file-pass pair can have multiple bugs.
        if (column == "    <tr>\n" or column == "    </tr>\n"):
          table_di_loc += column
          continue
        table_di_loc += "    <td>{0}</td>\n".format(column.strip())
      table_di_loc += "  <tr>\n"

  if not at_least_one_bug_found:
    table_di_loc += "  <tr>\n"
    table_di_loc += "    <td colspan='7'> No bugs found </td>\n"
    table_di_loc += "  </tr>\n"

  table_di_loc += "</table>\n"

  # Create the summary table for the loc bugs.
  table_di_loc_sum = "<table>\n"

  # Create the table's column headers.
  table_title_di_loc_sum = "DI Checker Summary of Location Bugs"
  table_di_loc_sum += "<caption><b>"
  table_di_loc_sum += table_title_di_loc_sum
  table_di_loc_sum += "</b></caption>\n"

  header_di_loc_sum = ["LLVM Pass Name", "Number of bugs"]
  table_di_loc_sum += "  <tr>\n"

  for column in header_di_loc_sum:
    table_di_loc_sum += "    <th>{0}</th>\n".format(column.strip())
  table_di_loc_sum += "  </tr>\n"

  # Print the summary.
  row = []
  for llvm_pass, num in sorted(di_location_bugs_summary.items()):
    row.append("    <tr>\n")
    row.append(llvm_pass)
    row.append(str(num))
    row.append("    </tr>\n")
  for column in row:
    if (column == "    <tr>\n" or column == "    </tr>\n"):
      table_di_loc_sum += column
      continue
    table_di_loc_sum += "    <td>{0}</td>\n".format(column.strip())
  table_di_loc_sum += "  <tr>\n"

  if not at_least_one_bug_found:
    table_di_loc_sum += "  <tr>\n"
    table_di_loc_sum += "    <td colspan='2'> No bugs found </td>\n"
    table_di_loc_sum += "  </tr>\n"
  table_di_loc_sum += "</table>\n"

  # Create the table for SP bugs.
  table_di_sp = "<table>\n"

  # Create the table's column headers.
  table_title_di_sp = "SP Bugs found by the LLVM DI Checker"
  table_di_sp += "<caption><b>"
  table_di_sp += table_title_di_sp
  table_di_sp += "</b></caption>\n"

  header_di_sp = ["File", "LLVM Pass Name", "Function Name", "Action"]
  table_di_sp += "  <tr>\n"

  for column in header_di_sp:
    table_di_sp += "    <th>{0}</th>\n".format(column.strip())
  table_di_sp += "  </tr>\n"

  at_least_one_bug_found = False

  # Handle loction bugs.
  for file, per_file_bugs in di_subprogram_bugs.items():
    for llvm_pass, per_pass_bugs in per_file_bugs.items():
      # No SP bugs for the pass.
      if len(per_pass_bugs) == 0:
        continue
      at_least_one_bug_found = True
      row = []
      table_di_sp += "  </tr>\n"
      # Get the bugs info.
      for x in per_pass_bugs:
        row.append("    <tr>\n")
        row.append(file)
        row.append(llvm_pass)
        row.append(x.fn_name)
        row.append(x.action)
        row.append("    </tr>\n")
      # Dump the bugs info into the table.
      for column in row:
        # The same file-pass pair can have multiple bugs.
        if (column == "    <tr>\n" or column == "    </tr>\n"):
          table_di_sp += column
          continue
        table_di_sp += "    <td>{0}</td>\n".format(column.strip())
      table_di_sp += "  <tr>\n"

  if not at_least_one_bug_found:
    table_di_sp += "  <tr>\n"
    table_di_sp += "    <td colspan='4'> No bugs found </td>\n"
    table_di_sp += "  </tr>\n"

  table_di_sp += "</table>\n"

  # Create the summary table for the sp bugs.
  table_di_sp_sum = "<table>\n"

  # Create the table's column headers.
  table_title_di_sp_sum = "DI Checker Summary of SP Bugs"
  table_di_sp_sum += "<caption><b>"
  table_di_sp_sum += table_title_di_sp_sum
  table_di_sp_sum += "</b></caption>\n"

  header_di_sp_sum = ["LLVM Pass Name", "Number of bugs"]
  table_di_sp_sum += "  <tr>\n"

  for column in header_di_sp_sum:
    table_di_sp_sum += "    <th>{0}</th>\n".format(column.strip())
  table_di_sp_sum += "  </tr>\n"

  # Print the summary.
  row = []
  for llvm_pass, num in sorted(di_sp_bugs_summary.items()):
    row.append("    <tr>\n")
    row.append(llvm_pass)
    row.append(str(num))
    row.append("    </tr>\n")
  for column in row:
    if (column == "    <tr>\n" or column == "    </tr>\n"):
      table_di_sp_sum += column
      continue
    table_di_sp_sum += "    <td>{0}</td>\n".format(column.strip())
  table_di_sp_sum += "  <tr>\n"

  if not at_least_one_bug_found:
    table_di_sp_sum += "  <tr>\n"
    table_di_sp_sum += "    <td colspan='2'> No bugs found </td>\n"
    table_di_sp_sum += "  </tr>\n"
  table_di_sp_sum += "</table>\n"


  # Finish the html page.
  html_footer = "</body>\n"
  html_footer += "</html>\n"
  new_line = "<br>\n"

  fileout.writelines(html_header)
  fileout.writelines(table_di_loc)
  fileout.writelines(new_line)
  fileout.writelines(table_di_loc_sum)
  fileout.writelines(new_line)
  fileout.writelines(new_line)
  fileout.writelines(table_di_sp)
  fileout.writelines(new_line)
  fileout.writelines(table_di_sp_sum)
  fileout.writelines(html_footer)
  fileout.close()

  print("The " + html_file + " generated.")

# Read the JSON file.
def get_json(file):
  json_parsed = None
  di_checker_data = []

  # The file contains json object per line.
  # An example of the line (formatted json):
  # {
  #  "file": "simple.c",
  #  "pass": "Deduce function attributes in RPO",
  #  "bugs": [
  #    [
  #      {
  #        "action": "drop",
  #        "metadata": "DISubprogram",
  #        "name": "fn2"
  #      },
  #      {
  #        "action": "drop",
  #        "metadata": "DISubprogram",
  #        "name": "fn1"
  #      }
  #    ]
  #  ]
  #}
  with open(file) as json_objects_file:
    for json_object_line in json_objects_file:
      try:
        json_object = loads(json_object_line)
      except:
        print ("error: No valid di-checker data found.")
        sys.exit(1)
      di_checker_data.append(json_object)

  return di_checker_data

# Parse the program arguments.
def parse_program_args(parser):
  parser.add_argument("file_name", type=str, help="json file to process")
  parser.add_argument("html_file", type=str, help="html file to output data")

  return parser.parse_args()

def Main():
  parser = argparse.ArgumentParser()
  opts = parse_program_args(parser)

  if not opts.html_file.endswith('.html'):
    print ("error: The output file must be '.html'.")
    sys.exit(1)

  debug_info_bugs = get_json(opts.file_name)

  # Use the defaultdict in order to make multidim dicts.
  di_location_bugs = defaultdict(lambda: defaultdict(dict))
  di_subprogram_bugs = defaultdict(lambda: defaultdict(dict))

  # Use the ordered dict to make a summary.
  di_location_bugs_summary = OrderedDict()
  di_sp_bugs_summary = OrderedDict()

  # Map the bugs into the file-pass pairs.
  for bugs_per_pass in debug_info_bugs:
    bugs_file = bugs_per_pass["file"]
    bugs_pass = bugs_per_pass["pass"]

    bugs = bugs_per_pass["bugs"][0]

    di_loc_bugs = []
    di_sp_bugs = []
    for bug in bugs:
      bugs_metadata = bug["metadata"]
      if bugs_metadata == "DILocation":
        action = bug["action"]
        bb_name = bug["bb-name"]
        fn_name = bug["fn-name"]
        instr = bug["instr"]
        di_loc_bugs.append(DILocBug(action, bb_name, fn_name, instr))

        # Fill the summary dict.
        if bugs_pass in di_location_bugs_summary:
          di_location_bugs_summary[bugs_pass] += 1
        else:
          di_location_bugs_summary[bugs_pass] = 1
      elif bugs_metadata == "DISubprogram":
        action = bug["action"]
        name = bug["name"]
        di_sp_bugs.append(DISPBug(action, name))

        # Fill the summary dict.
        if bugs_pass in di_sp_bugs_summary:
          di_sp_bugs_summary[bugs_pass] += 1
        else:
          di_sp_bugs_summary[bugs_pass] = 1
      else:
        print ("error: Only DILocation and DISubprogram are supported.")
        sys.exit(1)

    di_location_bugs[bugs_file][bugs_pass] = di_loc_bugs
    di_subprogram_bugs[bugs_file][bugs_pass] = di_sp_bugs

  generate_html_report(di_location_bugs, di_subprogram_bugs, di_location_bugs_summary, \
                       di_sp_bugs_summary, opts.html_file,)

if __name__ == "__main__":
  Main()
  sys.exit(0)
