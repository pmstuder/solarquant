<?php

//require_once "../classes/node.inc";
//require_once "../classes/ConsumptionDatum.inc";
//require_once "../classes/PowerDatum.inc";
require_once "../classes/solarUtility.inc";


class TrainingFile {

    var $id;
    var $filename;
    var $createdOn;
    var $title;
    var $notes;
    var $statusId;
    var $patternSetId;
    
    //TODO add a longtext field for contents? 4GB limit
    
    function add()
    {
    
    
		/* setup sql*/
		$sql = "insert into training_file (filename, created_on, title, notes, status_id, pattern_set_id) values (\"$this->filename\",\"$this->createdOn\",\"$this->title\",\"$this->notes\",$this->statusId,$this->patternSetId)";

		echo("add training file sql:". $sql. "<br>");
		
				//create utility
		$theUtility = new solarUtility;

		//break;
		/* execute sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("training file insert sql failed");
		
		$this->id = mysql_insert_id();
		

    
    }
    function delete()
    {
  
    	        $this->constructFromId();
    
		// setup sql
		$sql = "delete from training_file where training_file_id = ".$this->id;

		echo("delete sql:". $sql. "<br>");
		
				//create utility
		$theUtility = new solarUtility;

		//break
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("training file delete sql failed");
		 


		try {
			echo("inside transaction before unlink<br>");
			//remove from file system
			unlink("../emergent/output/".$this->filename);
			
			echo("after unlink<br>");
			
		} catch (Exception $e) {
			echo("exception at unlink:".  $e->getMessage(). "\n");
		}
	 /*	
*/

    
    }
    function constructFromId()
    {
    	    // setup sql
    	    $sql = "select training_file_id, filename, created_on, title, notes, status_id, pattern_set_id from training_file where training_file_id = ".$this->id;

    	    echo("construct sql:". $sql. "<br>");
    	    
    	    		//create utility
		$theUtility = new solarUtility;
    	    
		/* execute sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("training file construct sql failed");
		
		/* loop through results*/
		while ($row = mysql_fetch_array ($result))
		{
			$this->constructFromRow($row);
		}
		
		
    	    
    
    }
        function constructFromRow($row)
    {
    	$this->id = $row["training_file_id"];
    	$this->createdOn = $row["created_on"];
    	$this->filename = $row["filename"];
    	$this->title = $row["title"];
	$this->notes = $row["notes"];
	$this->statusId = $row["status_id"];
	$this->patternSetId = $row["pattern_set_id"]; 

    }
    
        function listAll($displayMode, $defaultTrainingFileId)
    {
    
    if ($displayMode == "fullPage"){

			/* table of entities*/
			echo("<table cellpadding='15' cellspacing='15' class='table table-striped' border='0'>\n");

			echo("<tr class='solar4' bgcolor='#ffffff'>");
		
			echo("<td align='center'>\n");
				echo ("ID");
			echo("</td>\n");
			
			echo("<td align='center'>\n");
				echo ("FILENAME");
			echo("</td>\n");

			echo("<td align='center'>\n");
				echo ("TITLE");
			echo("</td>\n");
			
			echo("<td align='center'>\n");
				echo ("CREATED");
			echo("</td>\n");

			echo("<td align='center'>\n");
				echo ("NOTES");
			echo("</td>\n");

			echo("<td align='center'>\n");
				echo ("STATUS ID");
			echo("</td>\n");

			echo("<td align='center'>\n");
				echo ("PATTERN SET ID");
			echo("</td>\n");
			
			echo("<td align='center'>\n");
				echo ("ACTION");
			echo("</td>\n");

			echo("</tr>\n");

		}
		elseif ($displayMode == "selectBox")
		{
			echo("<select name='trainingFileId' size='1'>\n");
			
			//if -1
			if ($defaultTrainingFileId <= 0)
			{
				echo("<option value='0'>Other\n");
			}
			
		}
		

		/* setup the sql*/
		$sql = "select training_file_id, filename, title, created_on, notes, status_id, pattern_set_id from training_file order by created_on desc";

				//create utility
		$theUtility = new solarUtility;

		/* execute the sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("listAll trainingFile select sql failed");
		
		//setup vars
		$toggle = 0;
		$theColor = "#BFBFBF";

		/* loop through results*/
		while ($row = mysql_fetch_array ($result))
		{
		

						
			//instantiate object
			$theTrainingFile = new TrainingFile();
			$theTrainingFile->constructFromRow($row);

			if ($displayMode == "fullPage"){

			echo("<tr class='solar5'>");

				echo("<td align='center'>\n");
					echo ($theTrainingFile->id);
				echo("</td>\n");
			
				echo("<td align='center'>\n");
					echo ($theTrainingFile->filename);
				echo("</td>\n");

				echo("<td align='center'>\n");
					echo ($theTrainingFile->title);
				echo("</td>\n");
				
				echo("<td align='center'>\n");
					echo ($theTrainingFile->createdOn);
				echo("</td>\n");
				
				echo("<td align='center'>\n");
					echo ($theTrainingFile->notes);
				echo("</td>\n");

				echo("<td align='center'>\n");
					echo ($theTrainingFile->statusId);
				echo("</td>\n");

				echo("<td align='center'>\n");
					echo ($theTrainingFile->patternSetId);
				echo("</td>\n");
				
				echo("<td align='center'>\n");
					
					echo(" <a href='trainingFileAction.php?function=delete&trainingFileId=");
					echo($theTrainingFile->id);
					echo("'><button type='button' class='btn btn-danger btn-xs'>Delete</button></a>\n");
				

					//echo(" <a href='trainingFileAction.php?function=edit&trainingFileId=");
					//echo($theTrainingFile->id);
					//echo("'><img src='../admin/images/edit_small.gif' border='0' alt=''></a>\n");
					
		
					
				
				echo("</td>\n");	
			
				echo("</tr>\n");
			}
			elseif ($displayMode == "selectBox"){


				if ($theTrainingFile->id == $defaultTrainingFileId)
				{
					echo("<option value='".$theTrainingFile->id."' selected>".$theTrainingFile->filename.":".$theTrainingFile->id."\n");
				}
				elseif ($theTrainingFile->id != $defaultTrainingFileId) {
					echo("<option value='".$theTrainingFile->id."'>".$theTrainingFile->filename.":".$theTrainingFile->id."\n");
				}
			}
		
		} /*end while*/

		if ($displayMode == "fullPage"){
				echo("</table>\n");
		}
		elseif ($displayMode == "selectBox"){
			echo("</select>\n");
		}

    
    }
    
    
}

?>
