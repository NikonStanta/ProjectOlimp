<?php
 
  function pwd($number)  
  { $letters = array(
'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','r','s','t','u','v','x','y','z',
'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','r','s','t','u','v','x','y','z',
'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','R','S','T','U','V','X','Y','Z',
'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','R','S','T','U','V','X','Y','Z',
'1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6','7','8','9','0','.','_','-','.','_','-');  
    shuffle($letters);
    $pass=$letters[0];
    for ($i=1;$i<$number;$i++) $pass.=$letters[$i];
    return $pass;  
  }  

echo pwd(8);

?>