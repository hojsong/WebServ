<?php
$targetDirectory = "uploads/";  // 파일이 저장될 디렉토리
$targetFile = $targetDirectory . basename($_FILES["fileToUpload"]["name"]);  // 업로드된 파일 경로

// 파일을 서버에 저장
if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $targetFile)) {
    echo "파일이 업로드되었습니다.";
} else {
    echo "파일 업로드 중 오류가 발생했습니다.";
}
?>