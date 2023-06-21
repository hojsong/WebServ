<?php

header ('Content-Type:text/html; charset=utf-8');

//post방식으로 보낸값들은 $_post라는 배열변수에 저장되고 값을하나씩빼온다고 생각하면됩니다.
$name=$_POST['name'];
$password=$_POST['pw'];
$gender=$_POST['rg'];
$message=$_POST['msg'];
$car=$_POST['sel'];

//textarea에서 입력한 값을 가진 $message는 줄바꿈문자를 \n으로 가지고있습니다 인식못합니다.
//그래서 <br>로 바꿔야하는데
// 이 계념자체를 기본으로 가지고있는 함수가있습니다

$message= nl2br($message); 
//nl을 br로 바꾼다는겁니다 2는 영어로 to를 의미합니다 ㅋㅋ


echo "<h2>$name</h2>";
echo "<h4>$password</h4>";
echo "<h4>$gender</h4>";
echo "<h4>$message</h4>";
echo "<h4>$car</h4>";

// http://ㅁㅁㅁ.co.kr/Web/postTest.php 이 데이타를 다가져와도 이렇게만 보입니다
//post방식의 성질이며 로그인처럼 데이타가 다보이면안되는 보안상에 좋습니다.

//배열로 전달 fruits
$fruits= $_POST['fruits'];
//모양은같아도 위에거랑 다름. 왜냐. 이건 배열이니까
//일단 배열의 갯수를 알아야한다

$num= count($fruits);
    echo "<h4>";
    for($i=0; $i<$num; $i++){
        echo "$fruits[$i],";
    }
    echo "</h4>";

    //hidden 타입으로 전달된 값
    $aa= $_POST['aa'];
    echo "$aa";

?>