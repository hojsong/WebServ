#!/Users/hojsong/.brew/bin/python3
import sys
import os
import base64
from io import BytesIO
from PIL import Image

# 환경 변수 읽기
request_method = os.environ.get("REQUEST_METHOD", "POST")

# 이미지 업로드 처리 (POST 방식인 경우)
uploaded_image = None
if request_method == "POST":
    content_length = int(os.environ.get("CONTENT_LENGTH", 0))
    post_data = sys.stdin.buffer.read(content_length)
    uploaded_image = Image.open(BytesIO(post_data))

# 이미지를 Base64로 인코드
image_buffer = BytesIO()
uploaded_image.save(image_buffer, format="PNG")
image_base64 = base64.b64encode(image_buffer.getvalue()).decode("utf-8")

# 응답 생성
print()  # 응답 헤더와 본문을 분리하는 빈 줄

html_response = f"""<!DOCTYPE html>
<html>
<head>
    <title>CGI Example</title>
</head>
<body>
    <h1>Uploaded Image</h1>
    <img src="data:image/jpeg;base64,{image_base64}" alt="Uploaded Image">
</body>
</html>
"""

print(html_response)