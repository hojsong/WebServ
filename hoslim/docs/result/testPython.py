#!/usr/bin/env python3
import cgi

# HTTP 헤더 출력
print("Content-Type: text/html")
print()

# 입력값 가져오기
form = cgi.FieldStorage()
name = form.getvalue("name")
age = form.getvalue("age")

# 입력값 확인용 출력
print("<p>이름: %s</p>" % name)
print("<p>나이: %s</p>" % age)
