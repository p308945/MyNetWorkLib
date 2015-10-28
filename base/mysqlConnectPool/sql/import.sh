#!/bin/sh
user="root"
pass="123123"
db="test"
mysql -u${user} -p${pass} $db < sql.sql
