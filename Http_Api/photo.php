<?php
$dir="image/";
$num = 0;
$file=scandir($dir);
foreach ($file as $key) {
	if(preg_match('/.jpg$/',$key))
	{
		if ($key!=NULL&&$key!="."&&$key!="..")
		{
			echo "<img width=\"10%\" height=\"20%\" src=\"image\\".$key."\">&emsp;";
			$keylist[$num] = $key;
			$num +=1;
			if ($num==9)
			{
				echo "</br>";
				$temp = 0;
				for ($temp = 0;$temp<9;$temp++) {
					$forblank = strlen($keylist[$temp]);
					for($y=(30-$forblank)/2;$y>0;$y--)
					{
						echo "&nbsp;";                                                                       //##生成空格 对齐
					}
					echo "<a href=\"image/".$keylist[$temp]."\">".$keylist[$temp]."</a>";	
					for($y=(30-$forblank)/2;$y>0;$y--)
					{
						echo "&nbsp;";                                                                       //##生成空格 对齐
					}
					echo "&nbsp;&nbsp;"; 
				}
				$num = 0;
				echo "</br>";
			}
		}
	}
}
?>
