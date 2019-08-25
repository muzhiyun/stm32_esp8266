<?php
if(isset($_FILES["file"])&&(($_FILES["file"]["type"] == "image/gif")|| ($_FILES["file"]["type"] == "image/jpeg")|| ($_FILES["file"]["type"] == "image/pjpeg"))&& ($_FILES["file"]["size"] < 2000000))
{
	$file= $_FILES["file"];
	$key = $file["name"];
}
else
{
	echo try_json_encode("403","Uploaded file is invalid");
	exit(0);
}


$pic = $file["name"];
$pics = substr($pic,0,strrpos($pic,"."));
if(preg_match("/^\d*$/",$pics)){
	$data = number_format($pics);
	$time = date("Y-m-d H:i:s",$pics);
	$path = "image/".$file["name"];

}
else
{
	echo try_json_encode("403","The filename is invalid");
	exit(0);
}


if (isset($_POST["x"])&&isset($_POST["y"])&&isset($_POST["h"]))
{
	$x = $_POST["x"];
	$y = $_POST["y"];
	$h = $_POST["h"];
}
else
{
	echo try_json_encode("403","The parameter is invalid");
	exit(0);
}


/*echo "</br>文件:".$pic;
echo "</br>x:".$x;
echo "</br>y:".$y;
echo "</br>h:".$h;
echo "</br>path:".$path;
echo "</br>time:".$time;*/

$file_name = $file["name"];
$tmp_file = $file["tmp_name"];

$error = $file['error'];
    if($error == 0){
        move_uploaded_file($tmp_file,"./image/".$file_name);
		require "connet.php";					//数据库部分
		$sql="INSERT INTO oil (x,y,h,path,time) VALUES ($x,$y,$h,'$path','$time')";
		$conn->query($sql);
		$conn->close();
		//echo "</br>sql:".$sql;
		echo try_json_encode("0","success");
    }
	else
	{
		echo try_json_encode("403","Uploaded file is illegal");
		exit(0);
	}
	

function try_json_encode($retcode_temp,$message_temp){
	$before=array("retcode"=>$retcode_temp,"message"=>$message_temp);
	$after=json_encode($before);
	return $after;
}


?>