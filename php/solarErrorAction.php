<?php

		//echo("in nodeAction");
		
		
		
//imports
require_once "../classes/SolarError.inc";

		//imports
//require "../classes/node.inc";

//echo("nodeAction after node import");

require_once "../classes/solarUtility.inc";


		//echo("nodeAction after all imports 2");
		
//break;


/* if there isn't an existing link */
if ($link = " "){
	/* create a link to the database*/
	//$link = mysql_connect ("mysql.fatcow.com","solar","solar") or die ("Could not connect1");
	
	//centralize authentication
	$theUtility = new solarUtility;
	$link = mysql_connect ($theUtility->dbHost,$theUtility->dbUser,$theUtility->dbPassword) or die ("Could not connect1");
}

//catch POSTed vars
if ($function == "")
{
	$function = trim($_REQUEST['function']);
}
if ($displayMode == "")
{
	$displayMode = trim($_REQUEST['displayMode']);
}
if ($theButton == "")
{
	$theButton = trim($_REQUEST['theButton']);
}
	

/* function = list*/
	if ($function == "list")
	{

		//instantiate object
		$theError = new SolarError;

		echo("<html>");
		
		echo("<head>\n");
		echo("<title>solarquant.Admin</title>\n"); 
		echo("<link href='../css/solarStyle.css' type='text/css' rel='stylesheet'>");
		echo("<link href='../css/bootstrap.min.css' type='text/css' rel='stylesheet'>");
		echo("<link href='../css/bootstrap-theme.min.css' type='text/css' rel='stylesheet'>");
		echo("<script src='../js/bootstrap.min.js'></script>");
				
		echo("</head>\n");
		
		echo("<body class='solar4' bgcolor='#ffffff'>");
		
		
		//call list function
		$theError->listAll("fullPage");
		
		echo("</body");
		echo("</html>");
		
		



	}
/* function = clear*/
	if ($function == "clear")
	{
		//instantiate object
		$theError = new SolarError;
		
				//call list function
		$theError->clear();
		
		//call list function
		$theError->listAll("fullPage");
		
	}




?>
