<?php
if(isset($_FILES["file"])&&(($_FILES["file"]["type"] == "image/gif")|| ($_FILES["file"]["type"] == "image/jpeg")|| ($_FILES["file"]["type"] == "image/pjpeg"))&& ($_FILES["file"]["size"] < 2000000))
{
	$file= $_FILES["file"];
	$key = $file["name"];
}
else
{
	echo try_json_encode("403","Uploaded file is illegal");
	exit(0);
}



$file_name = $file["name"];
$tmp_file = $file["tmp_name"];
echo $tmp_file;
$error = $file['error'];
    if($error == 0){
        move_uploaded_file($tmp_file,$file_name);
		echo try_json_encode("0","success");
    }
	

function try_json_encode($retcode_temp,$message_temp){
	$before=array("retcode"=>$retcode_temp,"message"=>$message_temp);
	$after=json_encode($before);
	return $after;
}


?>