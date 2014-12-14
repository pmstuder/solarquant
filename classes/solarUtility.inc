<?php
class solarUtility {

    var $dbHost = "127.0.0.1";
    var $dbName = "solarquant";
    var $dbUser = "solarquant";
    var $dbPassword = "solarquant";
    var $localAbsolutePath = "/var/www/solarquant/";

	
	function trace($varName,$varValue)
	{
	
		echo($varName .":".$varValue."<br>");
	
	}

	
function cleanQuotes( $value )
{
    if( get_magic_quotes_gpc() )
    {
          $value = stripslashes( $value );
    }
    //check if this function exists
    if( function_exists( "mysql_real_escape_string" ) )
    {
          $value = mysql_real_escape_string( $value );
    }
    //for PHP version < 4.3.0 use addslashes
    else
    {
          $value = addslashes( $value );
    }
    return $value;
}

function getSolarNetAuthentication($nodeId)
{
	
	echo "in getSolarNetAuthentication nodeId :".$nodeId."<br />";
	
	$nodeAuthentication = array();
	$nodeIdMarker = 0;
	
	// Parse with sections
	$authenticationArray = parse_ini_file($this->localAbsolutePath."ini/SolarNet.ini", true);

	
	foreach ($authenticationArray as $itemName => $itemValue)
	{
		
		//echo "for each itemName :".$itemName."<br />"; 
		
		//print_r($itemValue);
			
		//echo "<br> sizeof itemValue:".sizeof($itemValue)."<br />"; 
			
		if ($itemName == "nodes")
		{
			
			foreach ($itemValue as $nodeName => $nodesArray)
			{
				//$nodesArray = $itemValue;		
				$sizeOfNodesArray = sizeof($nodesArray);
			
				//echo "nodeName :".$nodeName."<br />";
				//echo "nodesArray :".$nodesArray."<br />";
				//$sizeOfNodeValue = sizeof($nodeValue);
				
				//echo "<br> sizeOfNodesArray:".$sizeOfNodesArray."<br />"; 
				
				$n = 0;
				$foundNodeId = false;
				
				foreach ($nodesArray as $nodeNum => $thisNodeId)
				{
					
					echo "thisNodeId :".$thisNodeId."<br />";
					echo "nodeId :".$nodeId."<br />";
					
					
					if ($thisNodeId == $nodeId)
					{
						//this n is the one to save
						$nodeIdMarker = $n;
						$foundNodeId = true;
						
						echo "found the nodeId at :".$n."<br />";
							
					}
					else
					{
						echo "NOT found the nodeId at :".$n."<br />";
					}
					
					//increment
					$n++;
					
					/*
					
					$n = 0;
					$foundNodeId = false;
					
					while ($n < $sizeOfNodesArray) & ($foundNodeId == false)
					{
						if ($nodesArray[$n] == $nodeId)
						{
							//this n is the one to save
							$nodeIdMarker = $n;
							$foundNodeId = true;
							
						}
						//increment
						$n++
					}
					
			
					//increment
					$n++
					
					*/
						
				}
		
			}
			//echo "itemValue 0 0 :".$itemValue[0][0]."<br />"; 
			//echo "itemName :".$itemName."<br />";
			//echo "itemValue :".$itemValue."<br />"; 
			//echo "itemValue[0] :".$itemValue[0]."<br />";
			//echo "node[0] :".$node[0]."<br />";
			//echo "nodes[0] :".$nodes[0]."<br />";
			
			//echo "is_array nodesArray:".is_array($nodesArray)."<br />"; 
			//echo "sizeOfNodesArray :".$sizeOfNodesArray."<br />"; 
			
			/*
			
			
			 */
		}
		elseif ($itemName == "tokens")
		{
			
			//echo "now in item tokens <br />";
			
			foreach ($itemValue as $tokenName => $tokensArray)
			{
				foreach ($tokensArray as $tokenNum => $thisToken)
				{
					
					//echo "thisToken :".$thisToken."<br />";
					
				}
				
			}

			//push the right token
			$nodeAuthentication[] = $tokensArray[$nodeIdMarker];
		}
		elseif ($itemName == "secrets")
		{
			echo "now in item secrets <br />";
			
			foreach ($itemValue as $secretName => $secretsArray)
			{
				foreach ($secretsArray as $secretNum => $thisSecret)
				{
					
					//echo "thisSecret :".$thisSecret."<br />";
					
				}
				
			}

			//push the right token
			$nodeAuthentication[] = $secretsArray[$nodeIdMarker];		
		}		
		
		
	} //end foreach
	
	echo "nodeAuthentication[0] :".$nodeAuthentication[0]."<br />";
	echo "nodeAuthentication[1] :".$nodeAuthentication[1]."<br />";
	
	
	//hand back a two item array
	return $nodeAuthentication;

}

	//uses the input curl arguments to return an text string
	function cURLdownload($url, $headerArray) 
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
  	 	 

  	 	 
  	 	 //try to set the output
  	 	  if( !curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1) ) return "FAIL: curl_setopt(CURLOPT_RETURNTRANSFER)"; 
  	 	 
  	 	 
  	 	 
  	 	 
  	 	 //if headerArray is submitted
  	 	 if ( strlen(trim($headerArray)) > 0)
  	 	 {
  	 	 
  	 	 	  //try to set the header output flag
  	 	 	  if( !curl_setopt($ch, CURLOPT_HEADER, 0) ) return "FAIL: curl_setopt(CURLOPT_HEADER)"; 
  	 	 
  	 	 	 echo "before CURLOPT_HEADER <br />"; 
  	 	 	 
  	 	 	 //try to write the header of the message
  	 	 	 if( !curl_setopt($ch, CURLOPT_HTTPHEADER, $headerArray ) ) return "FAIL: curl_setopt(CURLOPT_HTTPHEADER)"; 
  	 	 
  	 	 	 echo "after CURLOPT_HTTPHEADER <br />"; 
  	 	 
  	 	 }
  	 	 else
  	 	 {
  	 	 	echo "did not set headerArray <br />";  
  	 	 }
  	 	 
  	 	 //execute the curl call
  	 	 $curlOutput = curl_exec($ch);
  	 	   	 	  	 	 
   	 	 
  	 	 // echo "after curlOutput :".$curlOutput."<br />"; 
  	 	  
  	 	 //ob_end_clean(); 
  	 	 
  	 	 //close everything up
  	 	 curl_close($ch); 
  	 	 //fclose($fp); 
  	 	 return $curlOutput; 
  	 	 
 	 
 	} 
 	 
 	
//function servername()
//{
//		$this->dbServerName = "127.0.0.1";
//		return $servername;
//}

}
?>