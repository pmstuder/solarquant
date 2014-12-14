<?php

require_once "../classes/node.inc";
require_once "../classes/ConsumptionDatum.inc";
require_once "../classes/PowerDatum.inc";
require_once "../classes/solarUtility.inc";
require_once "../classes/TrainingFile.inc";
//require "../classes/WeatherDatum.inc";

//patternSet statuses:

// -1 = flagged as incomplete
// 0 = not processed queued for processing
// 1 = currently in process (could be stuck)
// 2 = finished properly, data downloaded                                                                 
// 3 = 

//patternSetTypes:
//  0 = not set
//  1 = just consumption
//  2 = just power
//  3 = both consumption and power

//sourceIds are currently for one node only
//TODO need to expand this to per-node resolution

class PatternSet {

    var $id;
    var $name;
    var $startDate;
    var $endDate;
    var $notes;
    var $nodes = array();
    var $sourceIds;
    var $statusId;
    var $patternSetTypeId;

    function generateDataTableContentsOld($showSaveTime)
    {
    	//evaluate variables
    	$fileHeaderComments = "Datafile for PatternSet ".$this->id."Generated on ".$showSaveTime;
    	$inputDataTableName = "inputData_PatternSet".$this->id;
    	$dataName = "data";
    	
    	//loop through weather input rows
    	$getWeatherInputRowsSql = "SELECT
    	node_id,
    	start_sample,
    	uv_index_weight,
    	temperature_hotter_weight,
    	temperature_colder_weight,
    	sky_conditions_weight,
    	humidity_weight,
    	visibility_weight,
    	barometric_high_pressure_weight,
    	barometric_low_pressure_weight,
    	barometer_falling_weight,
    	barometer_rising_weight,
    	barometer_steady_weight,
    	time_of_day_weight,
    	day_of_year_weight,
    	kilowatt_hours_weight 
    	FROM weather_input_pattern
    	where pattern_set_id = ".$this->id;
    	
    	
    			
		//create utility
		$theUtility = new solarUtility;
    	
    	//execute sql
		$weatherInputResult = mysql_db_query($theUtility->dbName,"$getWeatherInputRowsSql") or die ("getWeatherInputRowsSql failed");
		
		$totalInputRows = mysql_num_rows($weatherInputResult);
		
		/* loop through results*/
		while ($row = mysql_fetch_array ($weatherInputResult))
		{
			
			$startDate = $row["startSample"];
			
		$startTime = strtotime($thePatternSet->startDate);
		$showStartDate = date("Ymd_His",$startTime);
		$endTime = strtotime($thePatternSet->endDate);
		$showEndDate = date("Ymd_His",$endTime);
			
			$nameColumnValues .= "\"N".$row["node_id"]."_".$showStartDate."_".$showEndDate."\"; ";
			
			$inputMatrixData .= 
			$row["uv_index_weight"].";".
			$row["temperature_hotter_weight"].";".
			$row["temperature_colder_weight"].";".
			$row["sky_conditions_weight"].";".
			$row["humidity_weight"].";".
			$row["visibility_weight"].";".
			$row["barometric_high_pressure_weight"].";".
			$row["barometric_low_pressure_weight"].";".
			$row["barometer_falling_weight"].";".
			$row["barometer_rising_weight"].";".
			$row["barometer_steady_weight"].";".
			$row["time_of_day_weight"].";".
			$row["day_of_year_weight"].";";
			
			$outputMatrixData .= $row["kilowatt_hours_weight"].";";
			

			
		}
    	
    	
    	
    	
    	//generate the file
		$theFile = "// ".$fileHeaderComments."
DataTable .projects[0].data.gp[0][0] { 
DataTableCols @.data = [3] {
String_Data @[0] { };
float_Data @[1] { };
float_Data @[2] { };
};
};
DataTable .projects[0].data.gp[0][0] {
name=\"".$inputDataTableName."\";
desc=;
data {
name=\"".$dataName."\";
desc=;
data {
name=\"data\";
el_typ=String_Data;
el_def=0;
String_Data @[0] {
name=\"Name\";
col_flags=SAVE_ROWS|SAVE_DATA;
is_matrix=0;
cell_geom{ 1;};
calc_expr {
expr=;
};
ar {
name=;
[".$totalInputRows."] ".$nameColumnValues."};
};
float_Data @[1] {
name=\"Input\";
col_flags=SAVE_ROWS|SAVE_DATA;
is_matrix=1;
cell_geom{ 13;1;};
calc_expr {
expr=;
};
ar {
name=;
[13 1 ".$totalInputRows."] ".$inputMatrixData."};
};
float_Data @[2] {
name=\"Output\";
col_flags=SAVE_ROWS|SAVE_DATA;
is_matrix=1;
cell_geom{ 1;1;};
calc_expr {
expr=;
};
ar {
name=;
[1 1 ".$totalInputRows."]".$outputMatrixData."};
};
};
data_flags=SAVE_ROWS|AUTO_CALC;
auto_load=NO_AUTO_LOAD;
auto_load_file=;
keygen 4 0=0;
};";
		
		return $theFile;
    }

     function generateDataTableContents($showSaveTime)
    {
    	//evaluate variables
    	$fileHeaderComments = "Datafile for PatternSet ".$this->id."Generated on ".$showSaveTime;
    	$inputDataTableName = "inputData_PatternSet".$this->id;
    	$dataName = "data";
    	
    	//loop through weather input rows
    	$getWeatherInputRowsSql = "SELECT
    	node_id,
    	start_sample,
    	end_sample,
    	uv_index_weight,
    	temperature_hotter_weight,
    	temperature_colder_weight,
    	sky_conditions_weight,
    	humidity_weight,
    	visibility_weight,
    	barometric_high_pressure_weight,
    	barometric_low_pressure_weight,
    	barometer_falling_weight,
    	barometer_rising_weight,
    	barometer_steady_weight,
    	time_of_day_weight,
    	day_of_year_weight,
    	kilowatt_hours_weight 
    	FROM weather_input_pattern";
    	
    	//TODO: check to see whether the pattern itself is actual or virtual
    	if ($this->id == 9)
    	{
    		$getWeatherInputRowsSql .= " where kilowatt_hours_weight = 0 ";
    	}
    	else 
    	{
    		$getWeatherInputRowsSql .= " where kilowatt_hours_weight > 0 ";
    	}
    	
    	$getWeatherInputRowsSql .= " and pattern_set_id = ".$this->id;
    	
    			
		//create utility
		$theUtility = new solarUtility;
    	
    	//execute sql
		$weatherInputResult = mysql_db_query($theUtility->dbName,"$getWeatherInputRowsSql") or die ("getWeatherInputRowsSql failed");
		
		echo(" getWeatherInputRowsSql:" . $getWeatherInputRowsSql."<br><br>");
		
		$totalInputRows = mysql_num_rows($weatherInputResult);
		
		echo(" totalInputRows:" . $totalInputRows."<br><br>");
		
		$theFileContents = "";
		
		$headerContents[] = "_H:";
		$headerContents[] = "$"."Name";
		$headerContents[] = "%Input[2:0,0]<2:13,1>";
		
		
		//generate header row
		for ($i = 1;$i<13;$i++)
		{
			$headerContents[] = "%Input[2:".$i.",0]";
		}
		
		$headerContents[] = "%Output[2:0,0]<2:1,1>";
		
		//add header to file content output
		$theFileContents .= implode("\t",$headerContents)."\n";
		
		/* loop through results*/
		while ($row = mysql_fetch_array ($weatherInputResult))
		{
			
			$startDate = $row["start_sample"];
			
			$endDate = $row["end_sample"];
			
			echo(" startDate:" . $startDate."<br><br>");	
			echo(" endDate:" . $endDate."<br><br>");	
			
		$startTime = strtotime($startDate);
		$showStartDate = date("Ymd_His",$startTime);
		$endTime = strtotime($endDate);
		$showEndDate = date("Ymd_His",$endTime);
			
			$rowContents = array();
			$rowContents[] = "_D:";
			$rowContents[] = "\"N".$row["node_id"]."_".$showStartDate."_".$showEndDate."\"";
			
			$rowContents[] = $row["uv_index_weight"];
			$rowContents[] = $row["temperature_hotter_weight"];
			$rowContents[] = $row["temperature_colder_weight"];
			$rowContents[] = $row["sky_conditions_weight"];
			$rowContents[] = $row["humidity_weight"];
			$rowContents[] = $row["visibility_weight"];
			$rowContents[] = $row["barometric_high_pressure_weight"];
			$rowContents[] = $row["barometric_low_pressure_weight"];
			$rowContents[] = $row["barometer_falling_weight"];
			$rowContents[] = $row["barometer_rising_weight"];
			$rowContents[] = $row["barometer_steady_weight"];
			$rowContents[] = $row["time_of_day_weight"];
			$rowContents[] = $row["day_of_year_weight"];
			$rowContents[] =  $row["kilowatt_hours_weight"];
			
			//add header to file content output
			$theFileContents .= implode("\t",$rowContents)."\n";
			
		}
    	
    	
    	
    	
    	//generate the file
		
		
		return $theFileContents;
    }
    
    function generateConsumptionDataTableContents($showSaveTime)
    {
    	//evaluate variables
    	$fileHeaderComments = "Datafile for PatternSet ".$this->id."Generated on ".$showSaveTime;
    	$inputDataTableName = "inputData_PatternSet".$this->id;
    	$dataName = "data";
    	
    	//loop through weather input rows
    	$getWeatherInputRowsSql = "SELECT
    	node_id,
    	start_datetime,
    	end_datetime,
    	time_of_day_weight,
    	day_of_year_weight,
    	is_monday_weight,
    	is_tuesday_weight,
    	is_wednesday_weight,
    	is_thursday_weight,
    	is_friday_weight,
    	is_saturday_weight,
    	is_sunday_weight,
    	barometric_pressure_weight,
    	humidity_outside_weight,
    	temperature_outside_weight,
    	is_condition_clear_weight,
    	is_condition_clear_night_weight,
    	is_condition_fewclouds_weight,
    	is_condition_fewcloudsnight_weight,
    	is_condition_fog_weight,
    	is_condition_overcast_weight,
    	is_condition_severealert_weight,
    	is_condition_showers_weight,
    	is_condition_showers_scattered_weight,
    	is_condition_snow_weight,
    	is_condition_storm_weight,
    	temperature_hotter_weight,
    	temperature_colder_weight,
    	kilowatt_hours_weight
    	FROM consumption_input_pattern";
    	
    	/*
    	//TODO: check to see whether the pattern itself is actual or virtual
    	if ($this->id == 9)
    	{
    		$getWeatherInputRowsSql .= " where kilowatt_hours_weight = 0 ";
    	}
    	else 
    	{
    		$getWeatherInputRowsSql .= " where kilowatt_hours_weight > 0 ";
    	}
    	*/
    	
    	$getWeatherInputRowsSql .= " WHERE node_id = ".$this->nodes[0];
    	
    	$getWeatherInputRowsSql .= " AND kilowatt_hours_weight > 0";
    	
    			
		//create utility
		$theUtility = new solarUtility;
    	
    	//execute sql
		$weatherInputResult = mysql_db_query($theUtility->dbName,"$getWeatherInputRowsSql") or die ("getWeatherInputRowsSql failed");
		
		echo(" getWeatherInputRowsSql:" . $getWeatherInputRowsSql."<br><br>");
		
		$totalInputRows = mysql_num_rows($weatherInputResult);
		
		echo(" totalInputRows:" . $totalInputRows."<br><br>");
		
		$theFileContents = "";
		
		$headerContents[] = "_H:";
		$headerContents[] = "$"."Name";
		$headerContents[] = "%Input[2:0,0]<2:25,1>";
		
		
		//generate header row
		for ($i = 1;$i<25;$i++)
		{
			$headerContents[] = "%Input[2:".$i.",0]";
		}
		
		$headerContents[] = "%Output[2:0,0]<2:1,1>";
		
		//add header to file content output
		$theFileContents .= implode("\t",$headerContents)."\n";
		
		/* loop through results*/
		while ($row = mysql_fetch_array ($weatherInputResult))
		{
			
			$startDate = $row["start_datetime"];
			
			$endDate = $row["end_datetime"];
			
			echo(" startDate:" . $startDate."<br><br>");	
			echo(" endDate:" . $endDate."<br><br>");	
			
		$startTime = strtotime($startDate);
		$showStartDate = date("Ymd_His",$startTime);
		$endTime = strtotime($endDate);
		$showEndDate = date("Ymd_His",$endTime);
			
			$rowContents = array();
			$rowContents[] = "_D:";
			$rowContents[] = "\"N".$row["node_id"]."_".$showStartDate."_".$showEndDate."\"";
			
			$rowContents[] = $row["time_of_day_weight"];
			$rowContents[] = $row["day_of_year_weight"];
			$rowContents[] = $row["is_monday_weight"];
			$rowContents[] = $row["is_tuesday_weight"];
			$rowContents[] = $row["is_wednesday_weight"];
			$rowContents[] = $row["is_thursday_weight"];
			$rowContents[] = $row["is_friday_weight"];
			$rowContents[] = $row["is_saturday_weight"];
			$rowContents[] = $row["is_sunday_weight"];
			$rowContents[] = $row["barometric_pressure_weight"];
			$rowContents[] = $row["humidity_outside_weight"];
			$rowContents[] = $row["temperature_outside_weight"];
			$rowContents[] = $row["is_condition_clear_weight"];
			
			$rowContents[] = $row["is_condition_clear_night_weight"];
			$rowContents[] = $row["is_condition_fewclouds_weight"];
			$rowContents[] = $row["is_condition_fewcloudsnight_weight"];
			$rowContents[] = $row["is_condition_fog_weight"];
			$rowContents[] = $row["is_condition_overcast_weight"];
			$rowContents[] = $row["is_condition_severealert_weight"];
			$rowContents[] = $row["is_condition_showers_scattered_weight"];
			$rowContents[] = $row["is_condition_clear_weight"];
			
			$rowContents[] = $row["is_condition_snow_weight"];
			$rowContents[] = $row["is_condition_storm_weight"];
			$rowContents[] = $row["temperature_hotter_weight"];
			$rowContents[] = $row["temperature_colder_weight"];
			$rowContents[] =  $row["kilowatt_hours_weight"];
			
			//add header to file content output
			$theFileContents .= implode("\t",$rowContents)."\n";
			
		}
    	
    	
    	
    	
    	//generate the file
		
		
		return $theFileContents;
    }
    
    function clearNNInputWeights()
    {
    	    
    	    echo "in clearNNInputWeights this->id :".$this->id."<br />"; 
    	    
    	//clear out weighted patterns
		$deleteWeightedPatterns = "delete from weather_input_pattern where pattern_set_id = ".$this->id;
		
				
		//create utility
		$theUtility = new solarUtility;
		
		//execute sql
		$deleteWeightsResult = mysql_db_query($theUtility->dbName,"$deleteWeightedPatterns") or die ("deleteWeightedPatterns failed");
		
    }
    function cURLcheckBasicFunctions() 
    { 
  if( !function_exists("curl_init") && 
      !function_exists("curl_setopt") && 
      !function_exists("curl_exec") && 
      !function_exists("curl_close") ) return false; 
  else return true;
  }
    
  //this checks it's own patternSetTypeId and pulls the right datum depending on what kind of patternSetTypeId it is
  function getMyDatum()
  {
  			//always pull weather but use the weatherNodeId for this node
		$this->getDatumFromSolarNet("Weather");
		
		
		
		echo("this->patternSetTypeId:". $this->patternSetTypeId. " <br>");
		
		
		//just consumption
		if ($this->patternSetTypeId == 1) 
		{
			$this->getDatumFromSolarNet("Consumption");
		}
		//just power
		elseif ($this->patternSetTypeId == 2) 
		{
			$this->getDatumFromSolarNet("Power");
		}
		//both consumption and power
		elseif ($this->patternSetTypeId == 3) 
		{
			$this->getDatumFromSolarNet("Power");
			$this->getDatumFromSolarNet("Consumption");
		}  
  	 
  	  
  }
  
  
    function getDatumFromSolarNet($datumType)
    {
    	  // echo("cURL functions are valid:". this->cURLcheckBasicFunctions()."<br>");
     	    
    	if (function_exists("curl_init")){
			echo("curl_init OK<br>");
		}
		else
		{
			echo("curl_init NOT OK<br>");;
		}
	
	   if (function_exists("curl_setopt")){
			echo("curl_setopt OK<br>");
		}
		else
		{
			echo("curl_setopt NOT OK<br>");;
		}
		
		if (function_exists("curl_exec")){
			echo("curl_exec OK<br>");
		}
		else
		{
			echo("curl_exec NOT OK<br>");;
		}
		if (function_exists("curl_close")){
			echo("curl_close OK<br>");
		}
		else
		{
			echo("curl_close NOT OK<br>");;
		}
		
		
		//create the GMT date in the right format
		$gmRightNow = gmdate('D, d M Y H:i:s \G\M\T');
		
		echo "gmRightNow :".$gmRightNow."<br />";
		
		
		
		//express start and end dates in YYYY-mm-dd format
		$theStartDate = $this->startDate;
		$theEndDate = $this->endDate;

		//test intercepting the start and end using time as well as date
		$theStartDate = "2014-01-01T00:00";
		$theEndDate = "2014-01-01T03:00";
		
		echo "hey theStartDate :".$theStartDate."<br />";
		echo "theEndDate :".$theEndDate."<br />";
		
		//create the parts of the URL
		$theProtocol = "https://";
		$theHost = "data.solarnetwork.net";
		//TODO: expand to multiple nodes within each patternSet
		$theNodeId = $this->nodes[0];
		
		
		//construct the full node object
		$theNode = new Node();
		$theNode->id = $theNodeId;
		$theNode->constructFromId();
		
		//get the sourceIds for this patternSet
		//TODO: get a solution that gets the sourceIds for multiple nodes
		$theSourceIds = $theNode->getSources($this->id);
		//$theSourceIds = array();
		//$theSourceIds[] = "Phase1";
		
		
		echo "theNode->id :".$theNode->id."<br />";
		echo "theNode->weatherNodeId :".$theNode->weatherNodeId."<br />";
		echo "theSourceIds :".$theSourceIds."<br />";
		
		//instantiate object
		$nodeAuthentication = array();
		$theUtility = new solarUtility;
		
		
		
		if ($datumType == "Weather")
		{
			echo "datumType == Weather <br />";
			$nodeAuthentication = $theUtility->getSolarNetAuthentication($theNode->weatherNodeId);
		}
		else
		{
			echo "datumType not Weather <br />";
			$nodeAuthentication = $theUtility->getSolarNetAuthentication($theNodeId);
		}
		
		if (sizeof($nodeAuthentication) == 2)
		{
			$theToken = $nodeAuthentication[0];
			$theSecret = $nodeAuthentication[1];
		}
		else
		{
			echo "setting NO credentials <br />";
		}
		
		//exit;
		
		
		//set first 3-hour chunk dates
		$chunkStartDate = $this->startDate."T00:00";
		
		$tempStartDate = $this->startDate." 00:00";
		$currentDate = strtotime($tempStartDate);
		$futureDate = $currentDate+(60*180);
		$chunkEndDate = date("Y-m-d\TH:i", $futureDate);
		
		echo "tempStartDate :".$tempStartDate."<br />";
		echo "chunkStartDate :".$chunkStartDate."<br />";
		echo "chunkEndDate :".$chunkEndDate."<br />";
		
		echo "strtotime(tempDate) :".strtotime($tempStartDate)."<br />";
		echo "futureDate :".$futureDate."<br />";
		echo "strtotime(this->endDate) :".strtotime($this->endDate)."<br />";
		
		$j = 0;
		
		
		 //start with the last chunkEndDate as chunkStartDate
		 $currentDate = strtotime($this->startDate);
		
		//while the chunkStartDate < theEndDate
		while ($currentDate < strtotime($this->endDate))
		{
		   

			 $futureDate = $currentDate+(60*180);
			 
			 $chunkStartDate = date("Y-m-d\TH:i", $currentDate);
			 $chunkEndDate = date("Y-m-d\TH:i", $futureDate);
			 
			 echo $datumType. "loop ".$j."<br />";
			 echo "chunkStartDate :".$chunkStartDate."<br />";
			 echo "chunkEndDate :".$chunkEndDate."<br />";
			 
			 //advance currentDate to the last futureDate
			 $currentDate = $futureDate;
			 
			 $j++;
			

		
		
		//calculate the next 3 hours segment
		
		//TODO define the URI based on query entity type
		if ($datumType == "Consumption")
		{
			//$theUri = "/solarquery/api/v1/sec/datum/query?aggregate=Hour&endDate=".$theEndDate."&nodeId=114&sourceIds=00077.1&startDate=".$theStartDate."&type=Consumption";
			//$theUri = "/solarquery/api/v1/sec/datum/list?endDate=".$theEndDate."&nodeId=".$theNodeId."&sourceIds=00077.1&startDate=".$theStartDate."&type=Consumption";
			
			//using chunk dates
			//$theUri = "/solarquery/api/v1/sec/datum/list?endDate=".$chunkEndDate."&nodeId=".$theNodeId."&sourceIds=00077.1&startDate=".$chunkStartDate."&type=Consumption";
			//20140825 trying sourceIds
			//$theUri = "/solarquery/api/v1/sec/datum/list?endDate=".$chunkEndDate."&nodeId=".$theNodeId."&sourceIds=".$theNode->sourceIds."&startDate=".$chunkStartDate."&type=Consumption&max=10";
			
			//new JSON API
			//$theUri = "/solarquery/api/v1/pub/datum/list?nodeId=".$theNodeId."&startDate=".$chunkStartDate."&endDate=".$chunkEndDate."&type=Consumption&sorts%5B0%5D.sortKey=created&sorts%5B1%5D.sortKey=source&offset=0&max=570";
			//TODO add the sourceIds
			//$theUri = "/solarquery/api/v1/pub/datum/list?nodeId=".$theNodeId."&sourceIds=".implode(",", $theSourceIds)."&startDate=".$chunkStartDate."&endDate=".$chunkEndDate."&type=Consumption&sorts%5B0%5D.sortKey=created&sorts%5B1%5D.sortKey=source&offset=0&max=570";
			$theUri = "/solarquery/api/v1/pub/datum/list?nodeId=".$theNodeId."&sourceIds=".implode(",", $theSourceIds)."&startDate=".$chunkStartDate."&endDate=".$chunkEndDate."&sorts%5B0%5D.sortKey=created&sorts%5B1%5D.sortKey=source&offset=0&max=570";
			
			// /solarquery/api/v1/pub/datum/list?nodeId=120&startDate=2014-10-16T06:00&endDate=2014-10-16T09:00&offset=0&max=190
		}
		elseif ($datumType == "Power")
		{
			//using chunk dates
			$theUri = "/solarquery/api/v1/sec/datum/list?endDate=".$chunkEndDate."&nodeId=".$theNodeId."&startDate=".$chunkStartDate."&type=Power";
			
		}
		elseif ($datumType == "Weather")
		{
			//determine number of 20 minute intervals in date range - 72 per day
			$diff = abs(strtotime($theEndDate) - strtotime($theStartDate));

			$years = floor($diff / (365*60*60*24));
			$months = floor(($diff - $years * 365*60*60*24) / (30*60*60*24));
			$days = floor(($diff - $years * 365*60*60*24 - $months*30*60*60*24)/ (60*60*24));

			echo "years :".$years."<br />";
			echo "months :".$months."<br />";
			echo "days :".$days."<br />";

			//$theUri = "/solarquery/api/v1/pub/datum/list?type=weather&locationId=301025&max=".($days*72)."&start=".$theStartDate."&sorts%5B0%5D.sortKey=created&sorts%5B0%5D.descending=true";
			//$theUri = "/solarquery/api/v1/pub/datum/list?type=weather&locationId=301025&max=".($days*72)."&start=".$theStartDate."&sorts%5B0%5D.sortKey=created&sorts%5B0%5D.descending=true";
			
			//$theUri = "/solarquery/api/v1/sec/datum/list?endDate=".$theEndDate."&max=".($days*72)."&nodeId=114&startDate=".$theStartDate."&type=weather";
			//$theUri = "/solarquery/api/v1/sec/datum/list?endDate=".$theEndDate."&nodeId=114&startDate=".$theStartDate."&type=weather";
			
			//$theUri = "/solarquery/api/v1/sec/datum/list?endDate=".$theEndDate."&locationId=301025&nodeId=114&startDate=".$theStartDate."&type=weather";
			
			//using chunk dates
			//$theUri = "/solarquery/api/v1/sec/datum/list?endDate=".$chunkEndDate."&locationId=301025&nodeId=114&startDate=".$chunkStartDate."&type=weather";
			
			//$theUri = "/solarquery/api/v1/sec/datum/list?endDate=".$chunkEndDate."&locationId=301025&nodeId=".$theNodeId."&startDate=".$chunkStartDate."&type=weather";
			
			//TODO make sure to use the weatherNodeId here
			//$theUri = "/solarquery/api/v1/sec/datum/list?endDate=".$chunkEndDate."&locationId=301025&nodeId=30&startDate=".$chunkStartDate."&type=weather";
			//$theUri = "/solarquery/api/v1/sec/datum/list?endDate=".$chunkEndDate."&locationId=301025&nodeId=".$theNode->weatherNodeId."&startDate=".$chunkStartDate."&type=weather";
			
			//the maximum number of weatehrdatum in 3 hours is going to be around 9
			
			//new JSON API
			//$theUri = "/solarquery/api/v1/pub/datum/list?type=Weather&locationId=11536811&max=10&sorts%5B0%5D.sortKey=created&sorts%5B0%5D.descending=false&startDate=".$chunkStartDate."&endDate=".$chunkEndDate."&max=10";
			
			$theUri = "/solarquery/api/v1/pub/location/datum/list?locationId=301025&sourceIds=NZ%20MetService&offset=0&max=10&startDate=".$chunkStartDate."&endDate=".$chunkEndDate;
			
			
			
		}
		///solarquery/api/v1/sec/datum/query?nodeId=114&aggregate=Hour&startDate=2014-02-01T02:00&endDate=2014-02-01T03:30&type=Consumption&sourceIds=00077.1
		
		//start with a blank messageDigest
		$messageDigest = "";
		
		//now create the message contents
		$messageDigest .= "GET\n";
		$messageDigest .= "\n";
		$messageDigest .= "\n";
		$messageDigest .= $gmRightNow."\n";
		$messageDigest .= $theUri;
		
		echo "messageDigest :".$messageDigest."<br />"; 
		
		//write messageDigest to a file
		$fp1 = fopen($theUtility->localAbsolutePath.'emergent/output/messageDigest.txt', 'w');
		fwrite($fp1, $messageDigest);
		fclose($fp1);

		#hash the message content
		$hashedContent = base64_encode(hash_hmac('sha1', $messageDigest, $theSecret, true));
		
		//$hashedContent = base64_encode(hash_hmac('sha1', $messageDigest, $theSecret, true));
		
		//$hashedContent = base64_encode(hash_hmac('sha1', $messageDigest, "'".$theSecret."'", true));
		
		//create the header array for the CURL call
		$headerArrayElement1 = "X-SN-Date: ".$gmRightNow;
		
		

		//dynamic token Works 
		$headerArrayElement2 = "Authorization: SolarNetworkWS $theToken:".$hashedContent;
		
		//$headerArrayElement2 = "Authorization: SolarNetworkWS ".$theToken.":".$hashedContent;
		$headerArray = array($headerArrayElement1,$headerArrayElement2);
		
		//set the file to output the resulting data		
		$file = $theUtility->localAbsolutePath."emergent/output/file1.csv";
		
		//create the URL to use in the header of the curl call
		$theUrl = $theProtocol.$theHost.$theUri;
		
		//create the raw cURL command line
		$theCurlCall = "curl -H '".$headerArray[0]."' -H '".$headerArray[1]."' '".$theUrl."'";
		

		
		//write the curl call to a file
		$fp2 = fopen($theUtility->localAbsolutePath.'emergent/output/theCurlCall.txt', 'w');
		fwrite($fp2, $theCurlCall);
		fclose($fp2);
		
		
		//download the data to the file
		//echo($this->cURLdownloadToFile($theUrl, $headerArray, $file));
		
		echo "about to call cURLdownload<br />"; 
		
		//$theJSONData = $this->cURLdownload($theUrl, $headerArray);
		$theJSONData = $theUtility->cURLdownload($theUrl, $headerArray);
		
		echo "after call cURLdownload<br />"; 
		
		echo "after cURLdownload theJSONData :".$theJSONData."<br />"; 
		
	
		$resultsArray = json_decode($theJSONData, true);
		
		echo "sizeof(resultsArray):".sizeof($resultsArray)."<br />"; 
		
		echo "after json_decode theJSONData :".var_dump($resultsArray)."<br />"; 
		
		//echo "after json_decode resultsArray[0] :".$resultsArray[0]."<br />"; 
		
		//$innerResults = $resultsArray[1];
		
		foreach ($resultsArray as $name => $value)
		{
			
			echo "name :".$name." value: ".$value."<br />"; 
			
			if ($name == "data")
			{
				//echo "found data array:".var_dump($value)."<br />"; 
				echo "found data array:<br />"; 
				

					
				//TODO branch based on entity type
				if ($datumType == "Consumption")
				{
				
					$sizeOfArray = sizeof($value);
					echo "Consumption sizeOfArray :".$sizeOfArray." chunkStartDate: ".$chunkStartDate." chunkEndDate:".$chunkStartDate."<br />"; 
					
					foreach ($value as $itemName => $itemValue)
					{
						if ($itemName == "results")
						{
							
							$sizeOfResultsArray = sizeof($itemValue);
							echo "sizeOfResultsArray :".$sizeOfResultsArray."<br />"; 
							
							echo "itemValue[0]watts:".$itemValue[0]['watts']."<br />";
							
							//loop through consumptionDatum
							for($i = 0; $i < $sizeOfResultsArray;$i++)
							{
								
								echo "itemValue[i]:".$itemValue[$i]."<br />";
								
								echo "itemValue[0]watts:".$itemValue[0]['watts']."<br />";
								
								
								echo "itemValue[0]watts:".$itemValue[0]['watts']."<br />";
								
								echo "itemValue[0]sourceId:".$itemValue[0]['sourceId']."<br />";
								echo "this->sourceIds:".$this->sourceIds."<br />";
								
								
								//TODO make sure we can use multiple values by looping through an array of sourceIds
								// use in_array()
								//if (trim($itemValue[$i]['sourceId']) == $this->sourceIds)
								if (in_array(trim($itemValue[$i]['sourceId']), $theSourceIds))
								{
									echo "found a correct sourceId:".$itemValue[$i]['sourceId']."<br />";

								
									//instantiate new ConsumptionDatum
									$thisConsumptionDatum = new ConsumptionDatum;
									
									//set values
									$thisConsumptionDatum->volts = 240;
									$thisConsumptionDatum->amps = ($itemValue[$i]['watts']/$thisConsumptionDatum->volts);
									$thisConsumptionDatum->nodeId = $this->nodes[0]; 
									//$thisConsumptionDatum->whenLogged = $value[$i]['localDate'].$value[$i]['localTime']."00";
									$thisConsumptionDatum->whenLogged = substr($itemValue[$i]['created'],0,19);
									
									$thisConsumptionDatum->whenEntered = substr($itemValue[$i]['posted'],0,19);
									
									echo "value[i]['created'] :".$itemValue[$i]['created']."<br />";
									
									echo "value[i]['posted'] :".$itemValue[$i]['posted']."<br />";
									
									echo "value[i]['watts'] :".$itemValue[$i]['watts']."<br />";
									
									echo "thisConsumptionDatum->amps :".$thisConsumptionDatum->amps."<br />";
									
									//echo "value[i]['localDate'] :".$value[$i]['localDate']."<br />";
									
									// "value[i]['localTime'] :".$value[$i]['localTime']."<br />";
									
									echo "thisConsumptionDatum->whenLogged ok :".$thisConsumptionDatum->whenLogged."<br />"; 
									
									//as long as we have a nodeId
									if ($thisConsumptionDatum->nodeId > 0)
									{
										
										//add this ConsumptionDatum to DB
										$thisConsumptionDatum->add();
										
										echo "just added row # :".$i."<br />"; 
										
									}
									else
									{
										echo "invalid data NodeId :".$nodeId."<br />"; 
									}
								
								}
								else //not the right sourceId
								{
									echo "not in theSourceIds:".$itemValue[$i]['sourceId']."<br />";
								}
								
								
							} //end for
					
						}
						
					} //end loop through 4
					

					//echo "value[0]sourceId:".$value[0]['sourceId']."<br />"; 
				
				} //Consumption
				elseif ($datumType == "Power")
				{
				
					$sizeOfArray = sizeof($value);
					echo "sizeOfArray :".$sizeOfArray."<br />"; 
					
					foreach ($value as $itemName => $itemValue)
					{
						if ($itemName == "results")
						{
							
							$sizeOfResultsArray = sizeof($itemValue);
							echo "sizeOfResultsArray :".$sizeOfResultsArray."<br />"; 
							
							echo "itemValue[0]watts:".$itemValue[0]['watts']."<br />";
							
							//loop through consumptionDatum
							for($i = 0; $i < $sizeOfResultsArray;$i++)
							{
								
								echo "itemValue[i]:".$itemValue[$i]."<br />";
								
								echo "itemValue[0]watts:".$itemValue[0]['watts']."<br />";
								
								
								echo "itemValue[i]watts:".$itemValue[$i]['watts']."<br />";
								
								
								
								//instantiate new ConsumptionDatum
								$thisPowerDatum = new PowerDatum;
								
								//set values
								$thisPowerDatum->volts = 240;
								$thisPowerDatum->amps = ($itemValue[$i]['watts']/$thisPowerDatum->volts);
								
								$thisPowerDatum->source = ($itemValue[$i]['sourceId']);
								
								$thisPowerDatum->nodeId = $this->nodes[0]; 
								//$thisConsumptionDatum->whenLogged = $value[$i]['localDate'].$value[$i]['localTime']."00";
								$thisPowerDatum->whenLogged = substr($itemValue[$i]['created'],0,19);
								
								$thisPowerDatum->whenEntered = substr($itemValue[$i]['posted'],0,19);
								
								echo "value[i]['created'] :".$itemValue[$i]['created']."<br />";
								
								echo "value[i]['posted'] :".$itemValue[$i]['posted']."<br />";
								
								//echo "value[i]['localDate'] :".$value[$i]['localDate']."<br />";
								
								// "value[i]['localTime'] :".$value[$i]['localTime']."<br />";
								
								echo "thisPowerDatum->whenLogged ok :".$thisPowerDatum->whenLogged."<br />"; 
								
								if ($thisPowerDatum->nodeId > 0)
								{
									
									//add this ConsumptionDatum to DB
									$thisPowerDatum->add();
									
									echo "just added row # :".$i."<br />"; 
									
								}
								else
								{
									echo "invalid data NodeId :".$nodeId."<br />"; 
								}
								
								
								
							} //end for
					
						}
						
					} //end loop through 4
					

					//echo "value[0]sourceId:".$value[0]['sourceId']."<br />"; 
				
				} //Consumption
				elseif ($datumType == "Weather")
				{
						
					$sizeOfArray = sizeof($value);
					echo "in weather sizeOfArray :".$sizeOfArray."<br />"; 
					
					
							foreach ($value as $name2 => $value2)
							{
								echo "name2 :".$name2." value2: ".$value2."<br />"; 
								
								if ($name2 == "results")
								{
									
									$sizeOfArray = sizeof($value2);
									echo "in results sizeOfArray :".$sizeOfArray."<br />"; 
									
									//loop through weatherDatum
									for($i = 0; $i < $sizeOfArray;$i++)
									{
										
										echo "value[0]barometricPressure:".$value2[0]['barometricPressure']."<br />";
										
										
										//instantiate new ConsumptionDatum
										//$thisWeatherDatum = new Test1Datum;
										$thisWeatherDatum = new WeatherDatum;
						
										//set values
										$thisWeatherDatum->barometricPressure = $value2[$i]['barometricPressure'];
										//$thisWeatherDatum->skyConditions = $value2[$i]['skyConditions'];
										$thisWeatherDatum->skyConditions = $value2[$i]['sky'];
										//$thisWeatherDatum->weatherCondition = $value2[$i]['condition'];
										$thisWeatherDatum->weatherCondition = $value2[$i]['sky'];
										//$thisWeatherDatum->temperatureCelsius = $value2[$i]['temperatureCelsius'];
										$thisWeatherDatum->temperatureCelsius = $value2[$i]['temp'];
										$thisWeatherDatum->humidity = $value2[$i]['humidity'];
										$thisWeatherDatum->whenLogged = substr($value2[$i]['created'], 0, -8); ;
										$thisWeatherDatum->nodeId = $this->nodes[0]; 
										
										echo "before weatherDatum add<br />"; 
										
										//double check that the datum is complete
										if ($thisWeatherDatum->humidity	!= "")
										{
											//add this ConsumptionDatum to DB
											$thisWeatherDatum->add();
										}
										
										echo "after weatherDatum add<br />"; 
										
										
										
									}
									
								}
							}
						
						//echo "value[1]barometricPressure:".$value[1]['barometricPressure']."<br />";
					
						//echo "value[i]barometricPressure:".$value[$i]['barometricPressure']."<br />";
						

						
				}
				
				
				//$dataArray = $value;
						
				
			}

			
			// This will loop three times:
			//     $name = inbox
			//     $name = sent
			//     $name = draft
			// ...with $value as the value of that property
    		}
    		
    		
		
    		} //end chunking loop
    		
		//foreach ($dataArray as $name => $value)
		//{
		//	echo "in dataArray name :".$name." value: ".$value."<br />"; 
		//}
    		
		//echo var_dump(json_decode($theJSONData);
		
		/*

			

		
		*/
	}
	
	//uses the input curl arguments to return an text string
	function cURLdownloadOld($url, $headerArray) 
	{ 
		
	echo "in cURLdownload url :".$url."<br />";
	echo "in cURLdownload headerArray :".$headerArray[0]."<br />"; 
	echo "in cURLdownload headerArray :".$headerArray[1]."<br />"; 
	echo "in cURLdownload file :".$file."<br />"; 
	
	 //ob_start();
	 
	  echo "after ob_start <br />"; 
	
	//initialize curl
  	 $ch = curl_init(); 
 
  	 echo "after curl_init :".$ch."<br />"; 
    
  	 	 
  	 	 if( !curl_setopt($ch, CURLOPT_URL, $url) ) 
  	 	 { 
  	 	 	 //fclose($fp); // to match fopen() 
  	 	 	 curl_close($ch); // to match curl_init() 
  	 	 	 return "FAIL: curl_setopt(CURLOPT_URL)<br>"; 
  	 	 } 
  	 	 
  	 	 
  	 	 //try to set the curl file
  	 	 //if( !curl_setopt($ch, CURLOPT_FILE, $fp) ) return "FAIL: curl_setopt(CURLOPT_FILE)"; 
  	 	 
  	 	 //echo "after CURLOPT_FILE <br />"; 
  	 	 
  	 	 //try to set the header output flag
  	 	 if( !curl_setopt($ch, CURLOPT_HEADER, 0) ) return "FAIL: curl_setopt(CURLOPT_HEADER)"; 
  	 	 
  	 	 //try to set the output
  	 	  if( !curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1) ) return "FAIL: curl_setopt(CURLOPT_RETURNTRANSFER)"; 
  	 	 
  	 	 
  	 	 echo "after CURLOPT_HEADER <br />"; 
  	 	 
  	 	 //try to write the header of the message
  	 	 if( !curl_setopt($ch, CURLOPT_HTTPHEADER, $headerArray ) ) return "FAIL: curl_setopt(CURLOPT_HTTPHEADER)"; 
  	 	 
  	 	 echo "after CURLOPT_HTTPHEADER <br />"; 
  	 	 
  	 	 //execute the curl call
  	 	 $curlOutput = curl_exec($ch);
  	 	   	 	  	 	 
   	 	 
  	 	 // echo "after curlOutput :".$curlOutput."<br />"; 
  	 	  
  	 	 //ob_end_clean(); 
  	 	 
  	 	 //close everything up
  	 	 curl_close($ch); 
  	 	 //fclose($fp); 
  	 	 return $curlOutput; 
  	 	 
 	 
 	} 
 	
	//uses the input curl arguments to fill up the file
	function cURLdownloadToFile($url, $headerArray, $file) 
	{ 
		
	echo "in cURLdownloadToFile url :".$url."<br />";
	echo "in cURLdownloadToFile headerArray :".$headerArray[0]."<br />"; 
	echo "in cURLdownloadToFile headerArray :".$headerArray[1]."<br />"; 
	echo "in cURLdownloadToFile file :".$file."<br />"; 
	
	//initialize curl
  	 $ch = curl_init(); 
 
  	 echo "after curl_init :".$ch."<br />"; 
 
  	 //open a file that would contain the results of the curl call
  	 $fp = fopen($file, "w"); 
    
  	 echo "after fopen :".$fp."<br />"; 
    
  	 //as long as we have a valid file handle
  	 if($fp) 
  	 { 
    
  	 	 echo "in fp OK :".$fp."<br />"; 
     
  	 	 
  	 	 if( !curl_setopt($ch, CURLOPT_URL, $url) ) 
  	 	 { 
  	 	 	 fclose($fp); // to match fopen() 
  	 	 	 curl_close($ch); // to match curl_init() 
  	 	 	 return "FAIL: curl_setopt(CURLOPT_URL)<br>"; 
  	 	 } 
  	 	 
  	 	 
  	 	 //try to set the curl file
  	 	 if( !curl_setopt($ch, CURLOPT_FILE, $fp) ) return "FAIL: curl_setopt(CURLOPT_FILE)"; 
  	 	 
  	 	 echo "after CURLOPT_FILE <br />"; 
  	 	 
  	 	 //try to set the header output flag
  	 	 if( !curl_setopt($ch, CURLOPT_HEADER, 0) ) return "FAIL: curl_setopt(CURLOPT_HEADER)"; 
  	 	 
  	 	 echo "after CURLOPT_HEADER <br />"; 
  	 	 
  	 	 //try to write the header of the message
  	 	 if( !curl_setopt($ch, CURLOPT_HTTPHEADER, $headerArray ) ) return "FAIL: curl_setopt(CURLOPT_HTTPHEADER)"; 
  	 	 
  	 	 echo "after CURLOPT_HTTPHEADER <br />"; 
  	 	 
  	 	 //execute the curl call
  	 	 if( !curl_exec($ch) ) return "FAIL: curl_exec()<br>"; 
  	 	 
  	 	 echo "after curl_exec <br />"; 
  	 	 
  	 	 //close everything up
  	 	 curl_close($ch); 
  	 	 fclose($fp); 
  	 	 return "SUCCESS: $file [$url]"; 
  	 } 
  	 else
  	 { 
  	 	 //file hanndle didn't work 
  	 	 echo "FAIL fopen :".$fp."<br />"; 
  	 	 return "FAIL: fopen()"; 
 	 }
 	 
 	 
 	} 
 	

    
    function constructFromId()
    {
    
    	/* setup sql*/
		$sql = "select pattern_set_id, start_date, end_date, pattern_set_name, notes, status_id, pattern_set_type_id from pattern_set where pattern_set_id = ".$this->id;
		
			
		//create utility
		$theUtility = new solarUtility;

		/* execute sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("node construct sql failed");
		
		/* loop through results*/
		while ($row = mysql_fetch_array ($result))
		{
			$this->constructFromRow($row);
		}
    
		    	/* setup sql*/
		$sql = "select node_id, sourceId from patternset_node_match where pattern_set_id = ".$this->id;
		
		//echo("patternset_node_match sql:". $sql. "<br>");
    
				/* execute sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("node construct sql failed");
		

		
		//clear it out
		$this->nodes = array();
		
		/* loop through results*/
		while ($row = mysql_fetch_array ($result))
		{
			$this->nodes[] = $row["node_id"];
			
					//echo("row[node_id]:". $row["node_id"]. "<br>");
					//echo("row[sourceId]:". $row["sourceId"]. "<br>");
		
			//set sourceIds once for this PatternSet
			//$this->sourceIds = $row["sourceIds"];
		
		}
		
    }
    
    
    function constructFromRow($row)
    {
    	$this->id = $row["pattern_set_id"];
    	$this->startDate = $row["start_date"];
    	$this->endDate = $row["end_date"];
    	$this->name = $row["pattern_set_name"];
	$this->notes = $row["notes"];
	$this->statusId = $row["status_id"];
	$this->patternSetTypeId = $row["pattern_set_type_id"]; 

    }
    
    function listAll($displayMode, $defaultPatternSetId)
    {
    
    if ($displayMode == "fullPage"){

			/* table of entities*/
			echo("<table cellpadding='15' cellspacing='15' class='table table-striped' border='0'>\n");

			echo("<tr class='solar4' bgcolor='#ffffff'>");
		
			echo("<td align='center'>\n");
				echo ("ID");
			echo("</td>\n");
			
			echo("<td align='center'>\n");
				echo ("NAME");
			echo("</td>\n");
			
			echo("<td align='center'>\n");
				echo ("START DATE");
			echo("</td>\n");
			
			echo("<td align='center'>\n");
				echo ("END DATE");
			echo("</td>\n");

			echo("<td align='center'>\n");
				echo ("NOTES");
			echo("</td>\n");

			echo("<td align='center'>\n");
				echo ("STATUS");
			echo("</td>\n");

			echo("<td align='center'>\n");
				echo ("TYPE");
			echo("</td>\n");
			
			echo("<td align='center'>\n");
				echo ("ACTION");
			echo("</td>\n");

			echo("</tr>\n");

		}
		elseif ($displayMode == "selectBox")
		{
			echo("<select name='patternSetId' size='1'>\n");
			
			//if -1
			if ($defaultPatternSetId <= 0)
			{
				echo("<option value='0'>Other\n");
			}
			
		}
		

		/* setup the sql*/
		$sql = "select pattern_set_id, start_date, end_date, pattern_set_name, notes, status_id, pattern_set_type_id from pattern_set order by pattern_set_id desc";

				
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
			$thePatternSet = new PatternSet();
			$thePatternSet->constructFromRow($row);

			if ($displayMode == "fullPage"){

			echo("<tr class='solar4'>");

				echo("<td align='center'>\n");
					echo ($thePatternSet->id);
				echo("</td>\n");
			
				echo("<td align='center'>\n");
					echo ($thePatternSet->name);
				echo("</td>\n");

				echo("<td align='center'>\n");
					echo ($thePatternSet->startDate);
				echo("</td>\n");
				
				echo("<td align='center'>\n");
					echo ($thePatternSet->endDate);
				echo("</td>\n");
				
				echo("<td align='center'>\n");
					echo ($thePatternSet->notes);
				echo("</td>\n");

				echo("<td align='center'>\n");
					echo ($thePatternSet->statusId);
				echo("</td>\n");

				echo("<td align='center'>\n");
					echo ($thePatternSet->patternSetTypeId);
				echo("</td>\n");
				
				echo("<td align='center'>\n");


					echo(" <a href='patternSetAction.php?function=edit&patternSetId=");
					echo($thePatternSet->id);
					echo("'><button type='button' class='btn btn-success btn-xs'>Edit</button></a>\n");


					
					echo(" <a href='patternSetAction.php?function=pullFromSolarNet&patternSetId=");
					echo($thePatternSet->id);
					echo("'><span class'solar4'><button type='button' class='btn btn-success btn-xs'>Download</button></a>\n");
					
					echo(" <a href='patternSetAction.php?function=generateConsumptionNNWeights&patternSetId=");
					echo($thePatternSet->id);
					echo("'><button type='button' class='btn btn-success btn-xs'>Weights</button></a>\n");
					
					//echo(" <a href='patternSetAction.php?function=generateNNWeights&patternSetId=");
					//echo($thePatternSet->id);
					//echo("'><span class'solar4'>Generate NN Weights</a>\n");

					echo(" <a href='patternSetAction.php?function=createConsumptionDataTableFile&patternSetId=");
					echo($thePatternSet->id);
					echo("'><button type='button' class='btn btn-success btn-xs'>Datafile</button></a>\n");
					
					echo(" <a href='patternSetAction.php?function=clear&patternSetId=");
					echo($thePatternSet->id);
					echo("'><button type='button' class='btn btn-warning btn-xs'>Clear</button></a>\n");
					
					echo(" <a href='patternSetAction.php?function=delete&patternSetId=");
					echo($thePatternSet->id);
					echo("'><button type='button' class='btn btn-danger btn-xs'>Delete</button></a>\n");
					echo(" ");
					
					//echo(" <a href='patternSetAction.php?function=createDataTableFile&patternSetId=");
					//echo($thePatternSet->id);
					//echo("'><span class'solar4'>Create DataTableFile</a>\n");
				
				echo("</td>\n");	
			
				echo("</tr>\n");
			}
			elseif ($displayMode == "selectBox"){


				if ($thePatternSet->id == $defaultPatternSetId)
				{
					echo("<option value='".$thePatternSet->id."' selected>".$thePatternSet->name.":".$thePatternSet->id."\n");
				}
				elseif ($thePatternSet->id != $defaultPatternSetId) {
					echo("<option value='".$thePatternSet->id."'>".$thePatternSet->name.":".$thePatternSet->id."\n");
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
    function clear()
    {
    	    
    	    echo("in patternset clear<br>");
    	    
    	   
    	    	//setup sql
		$clearConsumptionDatumSQL = "delete from consumption_datum where node_id = ".$this->nodes[0];

		echo("clearConsumptionDatumSQL:". $clearConsumptionDatumSQL. "<br>");

				
		//create utility
		$theUtility = new solarUtility;
		
		//break;
		//execute sql
		$result = mysql_db_query($theUtility->dbName,"$clearConsumptionDatumSQL") or die ("insert sql failed");
 
		//setup sql
		$clearConsumptionInputPatternSQL = "delete from consumption_input_pattern  where pattern_set_id = ". $this->id;

		echo("clearConsumptionInputPatternSQL:". $clearConsumptionInputPatternSQL. "<br>");

		//break;
		//execute sql
		$result = mysql_db_query($theUtility->dbName,"$clearConsumptionInputPatternSQL") or die ("insert sql failed");
    	   
		 /*
		*/
		
	  echo("out patternset clear<br>");
	  
	  
	  
	 // exit;
    	    
    }
    function add()
    {
    
    
		/* setup sql*/
		$sql = "insert into pattern_set (pattern_set_name, start_date, end_date, status_id, pattern_set_type_id, notes) values (\"$this->name\",\"$this->startDate\",\"$this->endDate\",$this->statusId,$this->patternSetTypeId,\"$this->notes\")";

		echo("sql:". $sql. "<br>");
		
				
		//create utility
		$theUtility = new solarUtility;

		//break;
		/* execute sql*/
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("insert sql failed");
		
		$this->id = mysql_insert_id();
		
		//loop through nodes
		foreach ($this->nodes as $thisNode)
		{
			//make this the match
			$matchSql = "insert into patternset_node_match (pattern_set_id, node_id) values ($this->id,$thisNode)";
			
			echo("matchSql:". $matchSql. "<br>");
			
			// execute sql
			$result = mysql_db_query($theUtility->dbName,"$matchSql") or die ("insert match sql failed");
		
		}
    
    }
    
    function edit()
    {
    
		//construct from id
		$this->constructFromId();
		
		//echo("sourceIds:". $this->sourceIds. "<br>");
		
		//create a comma seperated text value from an array of nodeIds
		$nodeList = implode(",",$this->nodes);
		
		require_once('../classes/tc_calendar.php');
		
		/* generate edit form*/
		echo("<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.0 Transitional//EN'>\n");

		echo("<html>\n");
		echo("<head>\n");
		echo("<link href='../includes/calendar.css' rel='stylesheet' type='text/css' />");
		echo("<script language='javascript' src='../includes/calendar.js'></script>");
		echo("<title>solarnetwork.Admin</title>\n"); 
		echo("</head>\n");
		echo("<body bgcolor='#BFBFBF'>\n");

		echo("<form method=POST action='patternSetAction.php'>\n");
		
		echo("<table>");
		echo("<tr>");
			echo("<td bgcolor='#92999C'>Name</td>");
			echo("<td><input type='text' name='patternSetName' value='$this->name' size='60'></td>");
		echo("</tr>");
				echo("<tr>");
			echo("<td bgcolor='#92999C'>Start Date</td>");
			echo("<td>");
			
//instantiate class and set properties
$myCalendar = new tc_calendar("startDate", true);
$myCalendar->setIcon("images/iconCalendar.gif");
$myCalendar->setDate(substr($this->startDate,8,2), substr($this->startDate,5,2), substr($this->startDate,0,4));

//output the calendar
$myCalendar->writeScript();	 

			echo("</td>");
		echo("</tr>");
				echo("<tr>");
			echo("<td bgcolor='#92999C'>End Date</td>");
			echo("<td>");
			
				

//instantiate class and set properties
$myCalendar = new tc_calendar("endDate", true);
$myCalendar->setIcon("images/iconCalendar.gif");
$myCalendar->setDate(substr($this->endDate,8,2), substr($this->endDate,5,2), substr($this->endDate,0,4));

//output the calendar
$myCalendar->writeScript();	 
	


			echo("</td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td bgcolor='#92999C'>Notes</td>");
			echo("<td><textarea cols='70' rows='5' name='notes'>".$this->notes."</textarea></td>");
		echo("</tr>");
		echo("<tr>");
			echo("<td bgcolor='#92999C'>Nodes</td>");
			echo("<td>");
				
			        //loop through allSources
			       // for($i = 0; $i < sizeof($this->nodes);$i++)
			       // {
					 
					
			        	//construct the full node object
			        	$theNode = new Node();
			        	//$theNode->id = $this->nodes[$i];
			        	$theNode->id = $this->nodes[0];
			        	$theNode->constructFromId();
			        	
			        	echo($theNode->id."<br>");
			        	echo("<input type='hidden' name='nodeId' value='".$theNode->id."'><br><br>\n");
			        	
			        	echo($theNode->listSources("checkBox", $this->id)."<br>");
			        	
			        	
			        //}
			echo("</td>");
			//echo("<td><input type='text' name='nodeList' value='$nodeList' size='20'></td>");
		echo("</tr>");		
		echo("<tr>");
			echo("<td bgcolor='#92999C'>SourceIds</td>");
			//echo("<td><input type='text' name='sourceIds' value='$this->sourceIds' size='20'> a comma seperated list of sourceIds </td>");
			
			
		echo("</tr>");	
		echo("<tr>");
			echo("<td bgcolor='#92999C'>statusId</td>");
			echo("<td><input type='text' name='statusId' value='$this->statusId' size='10'> 0=queued Not Processed, 1=Currently In Process. 2=finished properly    </td>");
		echo("</tr>");	
		echo("<tr>");
			echo("<td bgcolor='#92999C'>patternSetTypeId</td>");
			echo("<td><input type='text' name='patternSetTypeId' value='$this->patternSetTypeId' size='10'> 1=Consumption, 2=Generation, 3=Generation+Consumption    </td>");
		echo("</tr>");	
		
		echo("</table>");
		

		echo("<input type='hidden' name='patternSetId' value='".$this->id."'><br><br>\n");
		echo("<input type='hidden' name='function' value='update'><br><br>\n");
		echo("<input type='submit' name='theButton' value='Update'>\n");
		echo("</form>\n");
		echo("</body>\n");
		echo("</html>\n");
    
    
    
    }
    //TODO make the array of nodes an actual object array where nodes have sourceIds
    function update($sourceIds)
    {
    
    	// setup sql
		$sql = "update pattern_set 
		set 
		pattern_set_name = '".$this->name."', 
		start_date = '".$this->startDate."', 
		end_date = '".$this->endDate."', 
		notes = '".$this->notes."',
		status_id = ".$this->statusId.",
		pattern_set_type_id = ".$this->patternSetTypeId." 
		where pattern_set_id = ".$this->id;

		
		//echo("sql:". $sql. "<br>");
		
				
		//create utility
		$theUtility = new solarUtility;
		
		// execute sql
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("patternSet update sql failed");
		
		//clear out match table
		$sql = "delete from patternset_node_match where pattern_set_id = ".$this->id;
    	
    	// execute sql
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("patternSet delete sql failed");
		
		//loop through nodes
		//TODO: make this actually deal with multiple nodes
		foreach ($this->nodes as $thisNode)
		{
			
			foreach ($sourceIds as $thisSourceId)
			{
			//make this the match
			$matchSql = "insert into patternset_node_match (pattern_set_id, sourceId, node_id) values ($this->id,\"$thisSourceId\",$thisNode)";
			
			echo("matchSql:". $matchSql. "<br>");
			
			// execute sql
			$result = mysql_db_query($theUtility->dbName,"$matchSql") or die ("insert match sql failed");
			
			//add the sources for this node
			}
		
		}
		
    
    }
    
    function delete()
    {
    	
    	$sql = "delete from patternset_node_match where pattern_set_id = ".$this->id;
    	
    			
		//create utility
		$theUtility = new solarUtility;
    	
    	// execute sql
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("patternSet delete sql failed");
		
	 	$sql = "delete from weather_input_pattern where pattern_set_id = ".$this->id;
    	
    	// execute sql
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("patternSet delete sql failed");
		
		$sql = "delete from pattern_set where pattern_set_id = ".$this->id;
    	
    	// execute sql
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("patternSet delete sql failed");
    
    }
    // write to the file system with a .DAT file that emergent can pick up
    function writeTrainingFile($fileContents)
    {
    	    
    	    	echo("in writeTrainingFile<br>");
    	    
    	  	echo(" fileContents:" . $fileContents."<br><br>");	
		
		//generate unique name based on datetime
		$startTime = strtotime($this->startDate);
		$showStartDate = date("Ymd",$startTime);
		$endTime = strtotime($this->endDate);
		$showEndDate = date("Ymd",$endTime);
				
		//name the file
		$fileName = "ConsumptionPattern_".$this->id."_".$showStartDate."_".$showEndDate."_".$saveTime.".dat";
		//$fileName = "S".$thePatternSet->id."_".$saveTime.".dtbl";
		
		echo(" fileName:" . $fileName."<br><br>");	
		
		$fp = fopen("../emergent/output/".$fileName, 'w');
		//$fp = fopen($fileName, 'w');
		
		//write the file
		fwrite($fp, $fileContents);

		//close the file
		fclose($fp);  
		
		echo("wrote fileName:" . $fileName." to file system <br><br>");
		
		
		
		//instantiate object
		$theTrainingFile = new TrainingFile;
		
		//set vars
		$saveTime = strtotime("now");
		$showSaveTime = date("Y-m-d H:i:S",$saveTime);
		
		$theTrainingFile->filename = $fileName;
		$theTrainingFile->createdOn = $showSaveTime;
		$theTrainingFile->title = $fileName;
		$theTrainingFile->patternSetId = $this->id;
		$theTrainingFile->statusId = 1;
		//$theTrainingFile->notes = $notes;
		
		
			//run update
			$theTrainingFile->add();
			
			echo("after theTrainingFile add<br>");
		/*	
			
    	    */
    	    
		echo("out writeTrainingFile<br>");
		
		
    	   
    	    
    }
    
}//end class

?>