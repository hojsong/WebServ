#!/usr/bin/env python3
import os
import sys

# 환경 변수 읽기
request_method = os.environ.get("REQUEST_METHOD", "POST")

# 입력 데이터 처리 (POST 방식인 경우)
if request_method == "POST":
    content_length = int(os.environ.get("CONTENT_LENGTH", 0))
    post_data = sys.stdin.read(content_length)

# 응답 생성
print()  # 응답 헤더와 본문을 분리하는 빈 줄

html_response = f"""<!DOCTYPE html>
<html>
<head>
    <title>CGI Program</title>
</head>
<body>
    <h1>Hello from CGI</h1>
    <p>Request Method: {request_method}</p>
    <p>Post Data: {post_data if request_method == "POST" else "N/A"}</p>
</body>
</html>
"""

print(html_response)