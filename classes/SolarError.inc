<?php


//imports
require_once "../classes/solarUtility.inc";


//echo("in node.php after imports 2<br>");

class SolarError {

    var $id;
    var $whenLogged;
    var $module;
    var $details;

    
    function constructFromId()
    {
    
    	/* setup sql*/
		$sql = "select solar_error_id, when_logged, module, details from solar_error where solar_error_id = ".$this->id;
		
		//echo("sql:". $sql. "<br>");
		//create utility
		$theUtility = new solarUtility;

		/* execute sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("node constructFromId sql failed");
		
		/* loop through results*/
		while ($row = mysql_fetch_array ($result))
		{
			$this->constructFromRow($row);
		}
    
    
    }
    
    function constructFromRow($row)
    {
    	$this->id = $row["solar_error_id"];
    	$this->whenLogged = $row["when_logged"];
    	$this->module = $row["module"];
	$this->details = $row["details"];
    }
    
    function add()
    {
    
    
		/* setup sql*/
		$sql = "insert into solar_error (when_logged, module, details) values (\"$this->whenLogged\",\"$this->module\",\"$this->details\")";

		//echo("sql:". $sql. "<br>");
		
				//create utility
		$theUtility = new solarUtility;

		//break;
		/* execute sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("insert sql failed");
		
		$this->id = mysql_insert_id();
		
    }
    function clear()
    {
    
    
		/* setup sql*/
		$sql = "delete from solar_error";

		//echo("sql:". $sql. "<br>");
		
				//create utility
		$theUtility = new solarUtility;

		//break;
		/* execute sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("delete sql failed");
		
		
		
    }    
    function listAll($displayMode)
    {
    
    	    if ($displayMode == "fullPage")
    	    {
    	    	    
    	    	    /* table of entities*/
    	    	    echo("<table cellpadding='15' cellspacing='15' class='table table-striped' border='0'>\n");
    	    	    
    	    	    echo("<tr class='solar4' bgcolor='#ffffff'>");
    	    	    
    	    	    echo("<td align='center'>\n");
    	    	    echo ("ID");
    	    	    echo("</td>\n");
    	    	    
    	    	    echo("<td  align='center'>\n");
    	    	    echo ("WHEN LOGGED");
    	    	    echo("</td>\n");
    	    	    
    	    	    echo("<td  align='center'>\n");
    	    	    echo ("MODULE");
    	    	    echo("</td>\n");
    	    	    
    	    	    echo("<td  align='center'>\n");
    	    	    echo ("DETAILS");
    	    	    echo("</td>\n");
    	    	    
    	    	    echo("</tr>\n");
    	    } //if fullpage
    	    	    
    	    	    		/* setup the sql*/
		$sql = "select solar_error_id, when_logged, module, details from solar_error order by when_logged desc";

				//create utility
		$theUtility = new solarUtility;

		/* execute the sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("listAll pattern_set select sql failed");
		
		//setup vars
		$toggle = 0;
		$theColor = "#BFBFBF";

		/* loop through results*/
		while ($row = mysql_fetch_array ($result))
		{
		

						
			//instantiate object
			$theError = new SolarError();
			$theError->constructFromRow($row);

			if ($displayMode == "fullPage"){

			echo("<tr>");

				echo("<td>\n");
					echo ($theError->id);
				echo("</td>\n");
			
				echo("<td>\n");
					echo ($theError->whenLogged);
				echo("</td>\n");

				echo("<td>\n");
					echo ($theError->module);
				echo("</td>\n");

				echo("<td>\n");
					echo ($theError->details);
				echo("</td>\n");
				
			echo("</tr>\n");

						
		} //displaymode


    	    	    
    	    } //end while
    	    
    	    		if ($displayMode == "fullPage"){
				echo("</table>\n");
		}
		
    }
    
}

?>
