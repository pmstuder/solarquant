<?php

class WeatherDatum {

    var $id;
    var $skyConditions;
    var $weatherCondition;
    var $temperatureCelsius = 0;
    var $humidity = 0;
    var $nodeId = 0;
    var $whenLogged;
    
    /*
    
    var $showRiseTime;
    var $showSetTime;
    var $showMidDayTime;
    var $kiloWattHoursPerM2 = 0;
    var $totalWattHours = 0;
    var $condition;
    var $skyConditions;
    var $reportingNodeId = 0;

    var $barometricPressure = 0;
    var $barometerDelta = 0;
    var $visibility = 0;
    var $uvIndex = 0;
   
    
    
     //pattern properties
     var $patternSetId;
    var $startSample;
    var $endSample;
    var $skyConditionsWeight = 0;
    var $temperatureHotterWeight = 0;
    var $temperatureColderWeight = 0;
    var $humidityWeight = 0;
    var $barometricHighPressureWeight = 0;
    var $barometricLowPressureWeight = 0;
    var $barometerFallingWeight = 0;
    var $barometerSteadyWeight = 0;
    var $barometerRisingWeight = 0;
    var $visibilityWeight = 0;
    var $uvIndexWeight = 0;
    var $timeOfDayWeight = 0;
    var $dayOfYearWeight = 0;
    var $totalWattHoursWeight = 0;
*/
    
    function add()
    {
    
    	    echo("in brand new WeatherDatum add<br>");
    	    
    	    	//setup sql
		$sql = "insert into weather_datum (sky_conditions, weather_condition, humidity, node_id, temperature_celsius, when_logged, when_entered) values (\"$this->skyConditions\",\"$this->weatherCondition\",$this->humidity,$this->nodeId,$this->temperatureCelsius,\"$this->whenLogged\",NOW())";

		echo("add weatherDatum:". $sql. "<br>");
		
				//create utility
		$theUtility = new solarUtility;

		//execute sql
		$result = mysql_db_query($theUtility->dbName,"$sql") or die ("insert sql failed3");
	   	

    
    }
    

    
} //end class


?>