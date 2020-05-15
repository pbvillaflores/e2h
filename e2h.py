#!/bin/env python
# transfer Teradata table to hive
#  - assumes hive user is the same as edw user. note the LOAD INPATH reference to user
#
# Example usage:
# edw_to_hive.py -e SOURCE_TABLE -d TERADATA_DB -l LOGMECH -u USERID -p PASS -sTERADTA_SEVER -b TARGET_TABLE


import sys
import re
import datetime
import argparse
from argparse import RawTextHelpFormatter
import smtplib
import getpass
import socket,time
DatalabName = "datalab_revassur"
multidelimit=False
FieldDelimiter = '^'
if multidelimit:
  FieldDelimiter = '^~'
e2hFolder='/data/e2h/'
notify_email = True


edwToHiveTypeMapping={
  "TIME":"STRING",
  "CHAR":"STRING",
  "VARCHAR":"STRING",
  "DATE":"DATE",
  "FLOAT":"DOUBLE",
  "INTEGER":"INT",
  "BYTEINT":"TINYINT",
  "SMALLINT":"SMALLINT",
  "BIGINT":"BIGINT",
  "NUMBER":"DECIMAL",
  "TIMESTAMP":"TIMESTAMP",
  "DECIMAL":"DECIMAL"
}

import subprocess, shlex
from pprint import pprint

def run_cmd(cmd):
    ''' runs the command and returns exit status '''
    p = subprocess.Popen(cmd, stderr=subprocess.STDOUT, stdout=subprocess.PIPE, shell=True, executable='/bin/bash')
    output = p.communicate()[0]
    return output,p.returncode

fromaddr = getpass.getuser()+"@"+socket.gethostname()
to = ['alerts@domain.com']

def alert(alert_text, alert_body):

  msg = 'from:'+fromaddr+'\r\n'
  msg += 'Subject:'+alert_text+' at ' +str(datetime.now())[:16] +'\r\n'
  msg += '\r\n\r\n\r\n' + alert_body.replace('\n','\r\n')
  s = smtplib.SMTP()
  s.connect('localhost')
  s.sendmail(fromaddr, to , 'To: ' + ','.join(to) + '\r\n' + msg + '\r\nFinished\r\n\r\n' )
  s.close()


parser = argparse.ArgumentParser(description="transfer edw table to hive", formatter_class=RawTextHelpFormatter)
parser.add_argument('-e', "--edwtable", type=str, help='EDW table', required=True )
parser.add_argument('-d', "--edwdb", type=str, help='EDW database', required=True )
parser.add_argument('-b', "--bdtable", type=str, help='BD table', required=True )
parser.add_argument('-l', "--logmech", type=str, help='logmech', required=True )
parser.add_argument('-u', "--user", type=str, help='user', required=True )
parser.add_argument('-p', "--password", type=str, help='password', required=True )
parser.add_argument('-r', "--hiveUser", type=str, help='BD user' )
parser.add_argument('-t', "--teraCreateUser", type=str, help='teradata create user' )
parser.add_argument('-x', "--teraCreateUserLogMech", type=str, help='teradata create user logmech' )
parser.add_argument('-v', "--teraCreateUserPassword", type=str, help='teradata create user pass' )
parser.add_argument('-c', "--credentialsHivePassword", type=str, help='BD password' )
parser.add_argument('-s', "--teradataserver", type=str, help='Teradata server', required=True )
parser.add_argument('-w', "--where", type=str, help='where clause EDW' )
parser.add_argument('-z', "--zexcludeFields", type=str, help='exclude field list, comma separated' )
parser.add_argument('-y', "--yusingThisTranslate", type=str, help='apply a particular translate for string default LATIN_TO_UNICODE' )
parser.add_argument('-k', "--kenanNRC", type=str, help='kenanNRC handling' )


args = parser.parse_args()

hiveuser = args.user
if args.hiveUser :
  hiveuser = args.hiveUser

teraCreateUser=args.user
if args.teraCreateUser :
  teraCreateUser = args.teraCreateUser

Teradata_Server = args.teradataserver
LogMech = ".LOGMECH "+args.logmech+"\n"

teraCreateUserLogMech = LogMech
if args.teraCreateUser :
  teraCreateUserLogMech = ".LOGMECH "+args.teraCreateUserLogMech+"\n"

teraCreateUserPassword = args.password
if args.teraCreateUser :
  teraCreateUserPassword = args.teraCreateUserPassword

def okBteqErr(retval, o):
  #a = 'Read from remote host ' in o and ': Connection timed out' in o
  return ('*** Ok, Session ' in o and '*** Query completed. ' in o and 'One column returned.' in o)

def ls_check(outfile):

  cmd2 = "ls -l " + e2hFolder +outfile
  o,retval = run_cmd(cmd2)

  if retval != 0:
    print 'retcode:',retval
    print o
    raise Exceoption, "Non-zero exit code on performing ls. Please review output."

  cmd2 = "kinit -kt ~/user.kt -p "+hiveuser+" ;hdfs dfs -ls " +outfile
  o2,retval2 = run_cmd(cmd2)

  if retval2 != 0:
    print 'retcode:',retval2
    print o2
    raise Exception, "Non-zero exit code on performing hdfs -ls. Please review output."

  q = o.strip().split("\n")[-1]
  q2 = o2.strip().split("\n")[-1]

  r = q.strip("\n").split(None)
  r2 = q2.strip("\n").split(None)

  # check that the output of ls have same size
  if r[4] != r2[4]:
    raise Exception, "file sizes inconsistent after dfs put. ls size: " + r[4] + "hdfs size: "+r2[4]

    #$ ls -l file1
    #-rw-r--r-- 1 username users 1506753489 Sep 14 20:38 file1
    #$ hdfs dfs -ls file1
    #-rw-r--r--   3 username username 1506753489 2016-09-14 20:39 file1



cmdLine = """bteq << EOF
  .SESSIONS 4
  .SET ERROROUT STDOUT;
  """ + teraCreateUserLogMech + ".LOGON " + Teradata_Server + "/"+teraCreateUser+","+ teraCreateUserPassword+ """;
  .SET WIDTH 9999


  /*Setting format of output file*/
  .SET RECORDMODE OFF;
  .SET FORMAT OFF ;
  .SET TITLEDASHES OFF;
  .SET SEPARATOR ',';
  CREATE TABLE """+DatalabName+".Tmp"+args.bdtable+" AS ( select * from "+args.edwdb+"."+args.edwtable+""" sample 1 ) WITH NO DATA;
  .quit
EOF"""

retries = 0
while True:
  o,retval = run_cmd(cmdLine)

  if '*** Error: Logon failed!' in o:
    # retry because this has been observed to fail even when login is correct.
    if retries < 5:
      retries += 1
      time.sleep(300)
      continue
    else:
      print 'Failing log...\n',o
      raise Exception, "Bteq Logon failed on create temp table: "+args.edwdb+"."+args.edwtable+"@" + Teradata_Server

  if retval != 0 and not okBteqErr(retval, o):
    print o
    raise Exception, "Non-zero exit code on performing teradata bteq exec CREATE TABLE."
  break



#########################################
# Now we do show table to get the SCHEMA
#########################################


cmdLine = """bteq << EOF
  .SESSIONS 4
  .SET ERROROUT STDOUT;
  """ + teraCreateUserLogMech + ".LOGON " + Teradata_Server + "/"+teraCreateUser+","+ teraCreateUserPassword+ """;
  .SET WIDTH 9999


  /*Setting format of output file*/
  .SET RECORDMODE OFF;
  .SET FORMAT OFF ;
  .SET TITLEDASHES OFF;
  .SET SEPARATOR ',';
  SHOW TABLE """+DatalabName+".Tmp"+args.bdtable+""";
  DROP TABLE """+DatalabName+".Tmp"+args.bdtable+""";
  .quit
EOF"""

retries = 0
while True:
  o,retval = run_cmd(cmdLine)

  if '*** Error: Logon failed!' in o:
    # retry because this has been observed to fail even when login is correct.
    if retries < 5:
      retries += 1
      time.sleep(300)
      continue
    else:
      raise Exception, "Bteq Logon failed on show table: "+args.edwdb+"."+args.edwtable+"@" + Teradata_Server

  if retval != 0 and not okBteqErr(retval, o):
    print o
    raise Exception, "Non-zero exit code on performing teradata bteq SHOW TABLE."
  break

# parse the show table output
state = 0
fields1=[]
select_fields=""
f_Str = ""
Sel_Expr=""
for i in o.split('\n'):
  if state == 0 and 'CREATE SET TABLE' in i:
    state = 1
  elif state == 1 and '(' == i.strip():
    state = 2
  elif state == 2:
    if 'PRIMARY ' in i or i.strip()[-1] == ';':
      break
    r = i.strip().split(None)
    
    # check if there is option to skip fields.
    if args.zexcludeFields:
      if r[0].lower() in args.zexcludeFields.lower().split(','):
        continue
      
    
    ds = r[1].strip()
    if ds[-1] == ',':
        ds = ds[:-1]
    k1 = k2 = 0
    for i in ds:
      if i == '(':
        k1+=1
      if i == ')':
        k2+=1
    while k2>k1  and  ds[-1] == ')':
      ds = ds[:-1]
      k2 -=1

    fieldName = r[0]
    datatypeString = ds
    fieldType = r[1].split('(')[0].split(',')[0].strip(')')

    HT = edwToHiveTypeMapping[ fieldType ]

    if fieldType == "DECIMAL" :
      HT = datatypeString

    if fieldType == "NUMBER" :
      HT = datatypeString.replace("NUMBER","DECIMAL")

    if fieldType == "TIME" :
      # Hive does not have a good match.Only DATE and TIMESTAMP
      # sample value: 20:15:14
      # so...
      HT = "STRING"

    fields1.append([fieldName,fieldType,datatypeString,HT] )

    # Build SELECT for BD CREATE
    select_fields += "  `" + fieldName.lower() + "`" + " " + HT+",\n"
    if len(fieldName)>128 :
      raise Exception, "FieldName cannot be more than 128 characters long: "+fieldName

    ##########################
    # Build EDW Sel_Expr
    ##########################
    if fieldType != "TIMESTAMP" and  fieldType != "DATE" :
      f_Str += "\nTRIM("

    # Handle string
    #fld = "'\"'||" + "COALESCE(" + 'FieldName' + ",'*NULL*')" + "||'\"'"
    #fld = "COALESCE(" + 'FieldName' + ",'*NULL*')"
    if args.yusingThisTranslate:
      fld = "COALESCE(TRANSLATE(" + fieldName + " USING "+args.yusingThisTranslate+" WITH ERROR),'*NULL*')"
    elif args.kenanNRC:
      fld = "COALESCE(CASE WHEN TRANSLATE_CHK("+fieldName+" USING UNICODE_TO_LATIN)>0 THEN NULL ELSE "+fieldName+" END,'*NULL*')"
      #above workaround for kenan.nrc extract
    else:
      fld = "COALESCE(TRANSLATE(" + fieldName + " USING LATIN_TO_UNICODE WITH ERROR),'*NULL*')"


    if fieldType == "BIGINT" :
      f_Str = f_Str +   "COALESCE(CAST(" + fieldName + " AS BIGINT),'?')"
    elif fieldType == "TIMESTAMP" :
      f_Str = f_Str +   "COALESCE(CAST(CAST(" + fieldName + " as format 'YYYY-MM-DDbHH:MI:SS') AS VARCHAR(19)),'?')"
    elif fieldType == "DATE" :
      f_Str = f_Str +   "COALESCE(CAST(CAST(" + fieldName + " as format 'YYYY-MM-DD') AS VARCHAR(10)),'?')"
    elif fieldType == "INTEGER" or fieldType == "BYTEINT" or fieldType == "SMALLINT" or fieldType == "FLOAT" or fieldType == "DECIMAL" or fieldType == "NUMBER" :
      f_Str = f_Str +   "COALESCE(" + fieldName + ",'?')"

    elif fieldType == "TIME" :
      # Hive does not have a good match.Only DATE and TIMESTAMP
      # sample value: 20:15:14
      # encountered in apollo.cdr  DWPRDE_VWXP_APOLLO.CDR rtng_event_time field
      # so...
      # HT = STRING
      f_Str = f_Str +   "COALESCE(CAST(" + fieldName + " AS VARCHAR(8)),'*NULL*')"
    else :
      f_Str = f_Str + fld

    if fieldType <> "TIMESTAMP" and fieldType <> "DATE" :
      Sel_Expr = f_Str + ") (TITLE '')"
      f_Str = f_Str + ")||'"+FieldDelimiter+"'||"
    else:
      Sel_Expr = f_Str + " (TITLE '')"
      f_Str = f_Str + "||'"+FieldDelimiter+"'||"
    ##########################
    # End of Build EDW Sel_Expr
    ##########################

select_fields=select_fields.rstrip().rstrip(',')+'\n'

print 'The select_fields for BD CREATE:'
print select_fields
print '\n\n'
print 'The Select expression for EDW export:'
print Sel_Expr
print '\n\n'

if len(args.bdtable) > 128:
  raise Exception, "HiveTable name cannot be more than 128 characters long: "+args.bdtable

######################$$$$$
# Now run the bteq export
######################$$$$$
print 'bteq export started of',args.edwdb+'.'+args.edwtable,'to bdtable:',args.bdtable
outfile= "e2h_"+args.bdtable+'_'+ str(datetime.datetime.now()).replace(' ','').replace(':','')

where_c=''
if args.where:
  where_c='\nWHERE '+args.where

cmdLine = """ bteq << EOF
  .SESSIONS 4
  .SET ERROROUT STDOUT;
  """ + LogMech + ".LOGON " + Teradata_Server + "/"+args.user+","+ args.password+ """;
  .SET WIDTH 9999
  .EXPORT RESET;
  .EXPORT REPORT FILE = """ + e2hFolder + outfile + """


  /*Setting format of output file*/
  .SET RECORDMODE OFF;
  .SET FORMAT OFF ;
  .SET TITLEDASHES OFF;
  .SET SEPARATOR ',';
  SELECT """ + Sel_Expr + " FROM " + args.edwdb+"."+args.edwtable + where_c+ """;
  .EXPORT RESET
  .quit
EOF"""
retries = 0
while True:
  o,retval = run_cmd(cmdLine)

  if '*** Error: Logon failed!' in o:
    # retry because this has been observed to fail even when login is correct.
    if retries < 5:
      retries += 1
      time.sleep(300)
      continue
    else:
      raise Exception, "Bteq Logon failed on export  table: "+args.edwdb+"."+args.edwtable+"@" + Teradata_Server

  if retval != 0 and not okBteqErr(retval, o):
    print o
    raise Exception, "Non-zero exit code on performing teradata export table."
  break

######################$$$$$
# End of run the bteq export
######################$$$$$

cmdLine = "/sbin/fuser " + e2hFolder + outfile

o,retval = run_cmd(cmdLine)
print 'fuser result:',o

print '############ check bteq export'
if retval == 1:   # 1 returned when there is no process
  print 'No process running. Good.'

print o
elif retval != 0:
  raise Exception, "Non-zero exit code on performing fuser. Please review output."
print '############ end of check bteq export'

else:
  q = o.strip().split('\n')[-1]
  r = q.split(None)
  pid = None

  if len(r) == 0 or not r[-1].isdigit():
    # there is possibility of this return result:
    '''
/.bashrc: line 10: unalias: ls: not found
8287file1:


<-- OR if multiple users but we should not have to care about this scenario:
$ fuser file1
file1:  8287 25703
    '''

    # handle the case where the pid is in front
    r = q.replace(e2hFolder+ outfile+':', '').split(None)
    if len(r) == 0 or not r[-1].isdigit():
      pid=None
    else:
      pid = int(r[-1])



  cmdLine = "pgrep bteq"

  while pid:

    if retval == 1:   # 1 returned when there is no match pgrep
      break

    if retval != 0:
      print 'output:',o
      raise Exception, "Non-zero exit code on performing pgrep. Please review output"
      break

    q = o.strip().split('\n')
    r = []
    if len(q)>0:
      r = [ int(x.strip()) for x in q if x.strip().isdigit() ]

    if len(r) == 0:
      break

    if pid in r:
      # verified there is a running bteq
      time.sleep(300)  # wait 5 minutes
      continue

    if notify_email:
      alert('bteq check out finished',args.edwtable)

    break


############ end of check bteq export

print 'Finished bteq export to '+outfile

cmdLine = ". ~/.bashrc; kinit -kt ~/user.kt -p "+hiveuser+"; hdfs dfs -put "+ e2hFolder + outfile

o,retval = run_cmd(cmdLine)

if retval != 0 :
  print o
  raise Exception, "Non-zero exit code on dfs put "+outfile

print 'Finished dfs put '+outfile

############ check dfs put


cmdLine = "/sbin/fuser " + e2hFolder + outfile
if retval == 1:   # 1 returned when there is no process
  ls_check(outfile)
  # we are done

if retval != 0:
  print 'output:',o
  raise Exception, "Non-zero exit code on performing fuser. Please review output."



q = o.strip().split('\n')[-1]
r = q.split(None)
pid = None
if len(r) == 0 or not r[-1].isdigit():

  # handle the case where the pid is in front
  r = q.replace(e2hFolder+ outfile+':', '').split(None)
  if len(r) == 0 or not r[-1].isdigit():
    ls_check(outfile)
    pid = None
  else:
    pid = int(r[-1])


cmdLine = "pgrep java | grep " + str(pid)
while pid:

  if retval == 1:  # 1 is returned when pid not found by grep
    ls_check(outfile)
    break

  if retval != 0:
    print 'output:',o
    raise Exception, "Non-zero exit code on performing pgrep. Please review output of dfs put."

  q = o.strip().split('\n')
  if len(q)>0:
    while not q[0].strip.isdigit():
      q = q[1:]
  r = [int(i.strip() ) for i in q]

  if len(r) == 0:
    ls_check(outfile)
    break

  if pid in r:
    # verified there is a running bteq/dfs put
    time.sleep(300)  # wait 5 minutes
    continue
  ls_check(outfile)
  break

############ end of check dfs put

cmdLine = 'bin/beeline.sh < /tmp/'+outfile

BD_DDL = """
  use datalab_ra;
  DROP TABLE IF EXISTS """+args.bdtable+"""_temp PURGE ;
  CREATE TABLE """+args.bdtable+"""_temp (
  """+ select_fields+ """
  )
  ROW FORMAT DELIMITED
  FIELDS TERMINATED BY '""" +FieldDelimiter+ """'
  --ESCAPED BY '\\'
  NULL DEFINED AS '*NULL*'
  STORED AS TEXTFILE ;
  LOAD DATA INPATH '/user/"""+ hiveuser+'/'+  outfile +"""'
  INTO TABLE """ + args.bdtable +"""_temp;

  DROP TABLE IF EXISTS """+args.bdtable+"""_temp2 PURGE ;
  CREATE TABLE """+args.bdtable+"""_temp2 AS
    SELECT * FROM """+args.bdtable+"""_temp ;
  DROP TABLE IF EXISTS """+args.bdtable+"""_temp PURGE ;
"""

if multidelimit:
  # 2017-10-06 I thought I might now use a new clever multidelimiter. But .java file is not avaialble on all nodes in BD cluster.
  BD_DDL="""
    use datalab_ra;
    DROP TABLE IF EXISTS """+args.bdtable+"""_temp PURGE ;

    CREATE TABLE """+args.bdtable+"""_temp (
    """+ select_fields+ '''
    )
    ROW FORMAT SERDE 'org.apache.hadoop.hive.contrib.serde2.MultiDelimitSerDe'
    WITH SERDEPROPERTIES ("field.delim"="'''+FieldDelimiter+ '''",'serialization.null.format'='*NULL*')
    STORED AS TEXTFILE;

    LOAD DATA INPATH '/user/'''+ hiveuser+'/'+  outfile +"""'
    INTO TABLE """ + args.bdtable +"""_temp;

    DROP TABLE IF EXISTS """+args.bdtable+"""_temp2 PURGE ;
    CREATE TABLE """+args.bdtable+"""_temp2 AS
      SELECT * FROM """+args.bdtable+"""_temp ;
    DROP TABLE IF EXISTS """+args.bdtable+"""_temp PURGE ;
    """

with open('/tmp/'+outfile,'w') as f:
  f.write(BD_DDL)

o,retval = run_cmd(cmdLine)

print o
if 'Error' in o or 'FAILED' in o or retval != 0:
  print cmdLine
  print
  raise Exception, "Non-zero exit code on CREATE TABLE temp target tables "+outfile





### now just do a rename of _temp2 table:

cmdLine = 'bin/beeline.sh < /tmp/'+outfile

BD_DDL = """
  use datalab_ra;
  DROP TABLE IF EXISTS """+args.bdtable+""" PURGE ;
  ALTER TABLE """+args.bdtable+"_temp2 RENAME TO "+args.bdtable+";\n"

with open('/tmp/'+outfile,'w') as f:
  f.write(BD_DDL)

o,retval = run_cmd(cmdLine)

if 'Error' in o or 'FAILED' in o or retval != 0:
  print cmdLine
  print
  print o
  raise Exception, "Non-zero exit code on RENAME temp2 to target tables "+outfile





print 'Done with TERADATA export of',args.edwdb+'.'+args.edwtable,'to Hive Table:',args.bdtable
print 'Final output:',o

