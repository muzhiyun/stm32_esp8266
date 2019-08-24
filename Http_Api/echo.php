<?php
//error_reporting(E_ALL);
///ini_set('display_errors', '1'); 

header("Access-Control-Allow-Origin: *");
$password=$_GET["password"];

if ($password!="*******")
{
	echo "<head>";
	echo "<title>此生无悔入华夏 来生愿在种花家</title>";
	//echo "<style>body {margin-left: 0px;margin-top: 0px;margin-right: 0px;margin-bottom: 0px;overflow: hidden;}</style>";
	echo "<meta id=\"viewport\" name=\"viewport\" content=\"width=device-width,minimum-scale=1.0,maximum-scale=1.0,user-scalable=no\">
    <style>
        html,body,iframe{width: 100%;height: 100%;padding: 0;margin: 0}
        #wrap{width: 100%;height: 100%;}
        iframe{border: none;}
    </style>";

	echo "</head>";
	echo "<body>";

	echo "<iframe src=\"https://news.163.com/\" width='100%' height='100%' frameborder='0' name=\"_blank\" id=\"_blank\"></iframe>";
	
	echo "</body>";
	exit();
}

function try_json_encode($id_temp,$connet_temp,$ip_temp,$time_temp){
	$before=array("num"=>$id_temp,"time"=>$connet_temp,"ip"=>$ip_temp);
	$after=json_encode($before);
	return $after;
}




require "connet.php";					//数据库部分
	$last="SELECT id,x,y,h,path,time FROM oil";
	$result = $conn->query($last);
	
	$arr = array();
	if($result && is_object($result)){
		while($res = $result->fetch_assoc()){
		$arr[] = $res;
		}
	}
	//print_r($arr);
	$jsonObj_1 = json_encode($arr);
	echo $jsonObj_1;
	$conn->close();

?>