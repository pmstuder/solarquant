<?php

//require "../classes/node.inc";

class PowerDatum {

    var $id;
    var $volts;
    var $nodeId;
    var $source;
    var $whenLogged;
    var $whenEntered;
    var $amps;
    var $notes;
    
    //var $nodes = array();
    
  
    function constructFromId()
    {
    
    	//setup sql 
		$sql = "select power_datum_id, volts, node_id, source, when_logged, when_entered, amps, notes from power_datum where power_datum_id = ".$this->id;
		
		echo(" PowerDatum constructFromId sql:". $sql. "<br>");
		
		//create utility
		$theUtility = new solarUtility;

		/* execute sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("powerDatum construct sql failed");
		
		/* loop through results*/
		while ($row = mysql_fetch_array ($result))
		{
			$this->constructFromRow($row);
		}
    
    
    }
    

    
  
    function constructFromRow($row)
    {
    	$this->id = $row["power_datum_id"];
    	$this->volts = $row["volts"];
	$this->nodeId = $row["node_id"];
	$this->source = $row["source"];
	$this->whenLogged = $row["when_logged"];
	
	echo("this->whenLogged:". $this->whenLogged. "<br>");
	
	$this->whenEntered = $row["when_entered"];
	$this->amps = $row["amps"];
	$this->notes = $row["notes"];

    }
    function add()
    {
    
    
		/* setup sql*/
		//$sql = "insert into consumption_datum (volts, node_id, when_logged, when_entered, amps, notes) values ($this->volts,$this->nodeId,\"$this->whenLogged\",NOW(),$this->amps,\"$this->notes\")";
		
		$sql = "insert into power_datum (volts, node_id, source, when_logged, when_entered, amps, notes) values ($this->volts,$this->nodeId,\"$this->source\",\"$this->whenLogged\",\"$this->whenEntered\",$this->amps,\"$this->notes\")";

		echo("add PowerDatum:". $sql. "<br>");

				//create utility
		$theUtility = new solarUtility;
		
		/* execute sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("insert sql failed3");
    
    }
   
    
}//end class

?>
