<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.3.1" name="tileset" tilewidth="16" tileheight="16" tilecount="1024" columns="32">
 <image source="../../resources/textures/map/tileset.png" width="512" height="512"/>
 <terraintypes>
  <terrain name="dirtRoad" tile="-1"/>
 </terraintypes>
 <tile id="0" terrain=",,,0"/>
 <tile id="1" terrain=",,0,0"/>
 <tile id="2" terrain=",,0,0"/>
 <tile id="3" terrain=",,0,"/>
 <tile id="4" terrain="0,0,0,"/>
 <tile id="5" terrain="0,0,,0"/>
 <tile id="6">
  <objectgroup draworder="index" id="2">
   <object id="5" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="7">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="11" width="16" height="5"/>
  </objectgroup>
 </tile>
 <tile id="8">
  <objectgroup draworder="index" id="2">
   <object id="3" type="Collision" x="0" y="4.90909" width="16" height="11.0909"/>
  </objectgroup>
 </tile>
 <tile id="9">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="1.90909" y="4.90909" width="14.0909" height="11.0909"/>
  </objectgroup>
 </tile>
 <tile id="10">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="11" width="16" height="5"/>
  </objectgroup>
 </tile>
 <tile id="11">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.181818" y="9" width="16" height="6.81818"/>
  </objectgroup>
 </tile>
 <tile id="32" terrain=",0,,0"/>
 <tile id="33" terrain="0,0,0,0">
  <objectgroup draworder="index" id="2">
   <object id="1" type="PuzzleGridRoad" x="7.5" y="7.5">
    <point/>
   </object>
  </objectgroup>
 </tile>
 <tile id="34" terrain="0,0,0,0">
  <objectgroup draworder="index" id="2">
   <object id="1" type="PuzzleGridRoad" x="7.5" y="8">
    <point/>
   </object>
  </objectgroup>
 </tile>
 <tile id="35" terrain="0,,0,"/>
 <tile id="36" terrain="0,,0,0"/>
 <tile id="37" terrain=",0,0,0"/>
 <tile id="64" terrain=",0,,0"/>
 <tile id="65" terrain="0,0,0,0">
  <objectgroup draworder="index" id="2">
   <object id="1" type="PuzzleGridRoad" x="6.5" y="9">
    <point/>
   </object>
  </objectgroup>
 </tile>
 <tile id="66" terrain="0,0,0,0">
  <objectgroup draworder="index" id="2">
   <object id="1" type="PuzzleGridRoad" x="9.25" y="9.75">
    <point/>
   </object>
  </objectgroup>
 </tile>
 <tile id="67" terrain="0,,0,"/>
 <tile id="68">
  <objectgroup draworder="index" id="2">
   <object id="1" type="PuzzleGridCollision" x="7.63636" y="9.27273">
    <point/>
   </object>
  </objectgroup>
 </tile>
 <tile id="70">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="2.18182" y="7.72727" width="13.8182" height="8.27273"/>
  </objectgroup>
 </tile>
 <tile id="71">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="2.90909" width="12.6364" height="13.0909"/>
  </objectgroup>
 </tile>
 <tile id="72">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0.863618" y="6.45455" width="14.5455" height="9.45454"/>
  </objectgroup>
 </tile>
 <tile id="73">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.181818" y="8.54546" width="16" height="7.18182"/>
  </objectgroup>
 </tile>
 <tile id="79">
  <objectgroup draworder="index" id="2">
   <object id="3" type="Collision" x="0" y="6" width="16" height="10"/>
  </objectgroup>
 </tile>
 <tile id="80">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="6" width="16" height="10"/>
  </objectgroup>
 </tile>
 <tile id="96" terrain=",0,,"/>
 <tile id="97" terrain="0,0,,"/>
 <tile id="98" terrain="0,0,,"/>
 <tile id="99" terrain="0,,,"/>
 <tile id="105">
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="0" width="16" height="3.81818"/>
  </objectgroup>
 </tile>
 <tile id="110">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="111">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="112">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="113">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="119">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0.318182" y="2.5" width="3.90909" height="13.3636"/>
   <object id="2" type="Collision" x="-0.136363" y="-0.181832" width="15.9091" height="2.63639"/>
   <object id="3" type="LightWall" x="1.77271" y="5.99998" width="2.45458" height="10.5455"/>
   <object id="4" type="LightWall" x="4.49998" y="5.45452" width="11.1819" height="3.00005"/>
  </objectgroup>
 </tile>
 <tile id="120">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.0454591" y="0.13635" width="15.9091" height="2.63639"/>
   <object id="2" type="LightWall" x="0" y="5.40907" width="16" height="3.00005"/>
  </objectgroup>
 </tile>
 <tile id="121">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0.04545" y="0.318169" width="15.9091" height="2.63639"/>
   <object id="2" type="LightWall" x="0" y="5.40907" width="16" height="3.00005"/>
  </objectgroup>
 </tile>
 <tile id="122">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.0454591" y="0.22726" width="15.9091" height="2.63639"/>
   <object id="2" type="LightWall" x="0" y="5.22725" width="16" height="3.00005"/>
  </objectgroup>
 </tile>
 <tile id="123">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.318186" y="0.0454414" width="15.9091" height="2.63639"/>
   <object id="2" type="LightWall" x="-5.90909e-05" y="5.40907" width="16" height="3.00005"/>
  </objectgroup>
 </tile>
 <tile id="124">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.136368" y="0.22726" width="15.9091" height="2.63639"/>
   <object id="2" type="Collision" x="12.7727" y="2.86362" width="3.00001" height="13"/>
   <object id="3" type="LightWall" x="-5.90909e-05" y="5.40907" width="16" height="3.00005"/>
   <object id="4" type="LightWall" x="12.6363" y="8.49997" width="3.27285" height="7.4546"/>
  </objectgroup>
 </tile>
 <tile id="129">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="131">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="5.90909" width="16" height="10.0909"/>
  </objectgroup>
 </tile>
 <tile id="134">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="4.81818" width="16" height="11.1818"/>
  </objectgroup>
 </tile>
 <tile id="135">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="4.90909" width="16" height="11.0909"/>
  </objectgroup>
 </tile>
 <tile id="142">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="143">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="144">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="145">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="151">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.136363" y="0.04545" width="3.90909" height="15.9091"/>
   <object id="2" type="LightWall" x="1.31816" y="-1.81818e-05" width="2.45458" height="15.6364"/>
  </objectgroup>
 </tile>
 <tile id="154">
  <objectgroup draworder="index" id="2">
   <object id="1" type="LightWall" x="12.909" y="0.227248" width="2.45466" height="6.00005"/>
   <object id="2" type="Collision" x="12.9545" y="0.22726" width="2.27274" height="6.45457"/>
  </objectgroup>
 </tile>
 <tile id="156">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="12.6818" y="0.136364" width="3.00001" height="15.8182"/>
   <object id="2" type="LightWall" x="12.6363" y="0.0908818" width="3.27285" height="15.8182"/>
  </objectgroup>
 </tile>
 <tile id="174">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="175">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="176">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="177">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="179">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="10" width="16" height="6"/>
  </objectgroup>
 </tile>
 <tile id="180">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="10" width="16" height="6"/>
  </objectgroup>
 </tile>
 <tile id="181">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="10" width="16" height="6"/>
  </objectgroup>
 </tile>
 <tile id="183">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0.045455" y="0.227268" width="3.90909" height="15.9091"/>
   <object id="2" type="LightWall" x="1.49998" y="0.1818" width="2.45458" height="15.6364"/>
  </objectgroup>
 </tile>
 <tile id="184">
  <objectgroup draworder="index" id="2">
   <object id="1" type="LightWall" x="0.0453909" y="13.2727" width="15.7274" height="2.5455"/>
  </objectgroup>
 </tile>
 <tile id="185">
  <objectgroup draworder="index" id="2">
   <object id="1" type="LightWall" x="-6.36364e-05" y="13.2727" width="15.7274" height="2.5455"/>
  </objectgroup>
 </tile>
 <tile id="188">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="12.5909" y="0.0909" width="3.00001" height="15.8182"/>
   <object id="2" type="LightWall" x="12.5454" y="0.0909" width="3.27285" height="15.8182"/>
  </objectgroup>
 </tile>
 <tile id="198">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="6" width="16" height="10"/>
  </objectgroup>
 </tile>
 <tile id="199">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="6" width="16" height="10"/>
  </objectgroup>
 </tile>
 <tile id="206">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0.181818" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="207">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="208">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="209">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="211">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="212">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="213">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="215">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.0909091" y="0.181818" width="3.90909" height="15.9091"/>
   <object id="2" type="LightWall" x="1.22726" y="0.318177" width="2.45458" height="15.6364"/>
  </objectgroup>
 </tile>
 <tile id="218">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="12.5454" y="4.0909" width="3.27275" height="11.5455"/>
   <object id="2" type="LightWall" x="13.2727" y="4.45453" width="2.00014" height="11.4546"/>
  </objectgroup>
 </tile>
 <tile id="219">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.0454818" y="4.45453" width="15.8182" height="3.27275"/>
   <object id="2" type="LightWall" x="0" y="4.18181" width="16" height="3.00003"/>
  </objectgroup>
 </tile>
 <tile id="220">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="12.4091" y="0.272718" width="3.00001" height="7.54547"/>
   <object id="2" type="Collision" x="-0.0454595" y="4.27272" width="12.4546" height="3.27275"/>
   <object id="3" type="LightWall" x="12.7272" y="0.0909" width="3.27285" height="7.18184"/>
   <object id="4" type="LightWall" x="0.181757" y="3.86363" width="12.4547" height="3.36366"/>
  </objectgroup>
 </tile>
 <tile id="227">
  <objectgroup draworder="index" id="2">
   <object id="2" type="Collision" x="3.45455" y="-1.54545" width="10.3636" height="10.0909">
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="247">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0.181818" y="4.44089e-16" width="15.6364" height="2.54545"/>
   <object id="2" type="LightWall" x="0.0909073" y="4.13636" width="15.6364" height="2.54546"/>
   <object id="3" type="LightWall" x="0.545436" y="-0.0909118" width="2.45458" height="3.9091"/>
  </objectgroup>
 </tile>
 <tile id="248">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0.272695" y="-0.0454777" width="15.5455" height="2.63641"/>
  </objectgroup>
 </tile>
 <tile id="249">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.136386" y="0.13634" width="15.5455" height="2.63641"/>
  </objectgroup>
 </tile>
 <tile id="250">
  <objectgroup draworder="index" id="2">
   <object id="2" type="Collision" x="-1.13636e-05" y="0.227249" width="15.7273" height="2.63641"/>
   <object id="3" type="LightWall" x="12.9999" y="-2.72727e-05" width="2.00014" height="7.00005"/>
   <object id="4" type="LightWall" x="-7e-05" y="4.49998" width="13.182" height="2.5455"/>
  </objectgroup>
 </tile>
 <tile id="259">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="262">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="4.63636" width="16" height="11.3636"/>
  </objectgroup>
 </tile>
 <tile id="263">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="4.81818" width="16" height="11.1818"/>
  </objectgroup>
 </tile>
 <tile id="311">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="13.2273" y="0.363636" width="3" height="16"/>
  </objectgroup>
 </tile>
 <tile id="312">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="5"/>
  </objectgroup>
 </tile>
 <tile id="322">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="10.7727" y="8.27273" width="5" height="7.36364"/>
  </objectgroup>
 </tile>
 <tile id="323">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="7.72727" width="12.4545" height="8.27273"/>
  </objectgroup>
 </tile>
 <tile id="326">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="327">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="344">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="3" height="16"/>
  </objectgroup>
 </tile>
 <tile id="376">
  <objectgroup draworder="index" id="3">
   <object id="2" type="Collision" x="-0.181818" y="-0.318182" width="16" height="2.27273"/>
  </objectgroup>
 </tile>
 <tile id="377">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.181818" y="13.4091" width="16" height="2.27273"/>
  </objectgroup>
 </tile>
 <tile id="422">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="423">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0.0909091" y="0.0909091" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="424">
  <objectgroup draworder="index" id="3">
   <object id="2" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="425">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="426">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="454">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="5"/>
  </objectgroup>
 </tile>
 <tile id="455">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="5"/>
  </objectgroup>
 </tile>
 <tile id="456">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="5"/>
  </objectgroup>
 </tile>
 <tile id="457">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="5"/>
  </objectgroup>
 </tile>
 <tile id="458">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="0" width="16" height="5"/>
  </objectgroup>
 </tile>
 <tile id="672">
  <objectgroup draworder="index" id="2">
   <object id="2" type="LightWall" x="13.0909" y="12.9545" width="2.90909" height="2.90909"/>
  </objectgroup>
 </tile>
 <tile id="673">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="13" width="16" height="12.1818"/>
   <object id="2" type="LightWall" x="0" y="11.0455" width="16" height="2.18183"/>
  </objectgroup>
 </tile>
 <tile id="674">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0" y="13" width="16" height="13.4545"/>
   <object id="3" type="LightWall" x="0" y="10.6818" width="16" height="2.18183"/>
  </objectgroup>
 </tile>
 <tile id="675">
  <objectgroup draworder="index" id="2">
   <object id="2" type="LightWall" x="0" y="12.3182" width="2.81818" height="3.54546"/>
  </objectgroup>
 </tile>
 <tile id="704">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="9.39773" y="6.72727" width="15.6591" height="11.4545"/>
   <object id="2" type="LightWall" x="12.6818" y="0.136359" width="1.54545" height="15.9091"/>
   <object id="3" type="Collision" x="10.5" y="-0.636363" width="13.8182" height="13.7273">
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="707">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-6.18182" y="7" width="12.9318" height="9"/>
   <object id="2" type="LightWall" x="0.227273" y="0.136359" width="2.27273" height="15.9091"/>
   <object id="3" type="Collision" x="-7.13636" y="0.0454564" width="13.7273" height="14.2727">
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="708">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="9.09091" y="0" width="10.7273" height="21.8182"/>
   <object id="2" type="Collision" x="-4.54545e-07" y="12.3636" width="8.90909" height="9.54545"/>
   <object id="3" type="LightWall" x="-0.0908905" y="11.8182" width="11.6363" height="1.27273"/>
   <object id="4" type="LightWall" x="12.7273" y="-0.272737" width="1.27273" height="8.8182"/>
   <object id="5" type="LightWall" x="9.27276" y="8.27273" width="4.09085" height="3.54546"/>
   <object id="6" type="Collision" x="5.59091" y="9.5" width="5.18182" height="5.18182">
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="710">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="1.09091" y="5.11364" width="15.0909" height="10.8181"/>
   <object id="2" type="LightWall" x="1.45455" y="1.15908" width="14.7273" height="14.7273"/>
   <object id="3" type="Collision" x="0.954545" y="0.318182" width="14.8182" height="15.9091">
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="736">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="8.625" y="0.125" width="15.5682" height="15.8182"/>
   <object id="2" type="LightWall" x="13.3182" y="0.136359" width="1.36364" height="15.9091"/>
  </objectgroup>
 </tile>
 <tile id="739">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-6.54545" y="0" width="13.2955" height="16"/>
   <object id="2" type="LightWall" x="0.0909091" y="0" width="2.36364" height="15.9091"/>
  </objectgroup>
 </tile>
 <tile id="740">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-5.18182" y="-0.0909091" width="12.2727" height="21.9091"/>
   <object id="2" type="Collision" x="7.18182" y="12.8182" width="8.72727" height="9.18182"/>
   <object id="3" type="LightWall" x="1.81818" y="-0.181827" width="1.27273" height="13.4546"/>
   <object id="4" type="LightWall" x="1.8182" y="12" width="14" height="1.27273"/>
  </objectgroup>
 </tile>
 <tile id="742">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="1.27274" y="0.18182" width="14.1818" height="15.4545"/>
   <object id="2" type="LightWall" x="1.54546" y="0.170452" width="14.1818" height="15.4008"/>
  </objectgroup>
 </tile>
 <tile id="768">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="9" y="0.0568182" width="14.6591" height="5"/>
   <object id="2" type="LightWall" x="13.1363" y="0.0454499" width="2.63636" height="2.54546"/>
   <object id="3" type="Collision" x="9.13637" y="2.63636" width="6.63636" height="6.90909">
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="769">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-1.36364" y="-11.7273" width="17.5454" height="21.7273"/>
   <object id="2" type="LightWall" x="0" y="0.499995" width="16" height="1.36365"/>
  </objectgroup>
 </tile>
 <tile id="770">
  <objectgroup draworder="index" id="2">
   <object id="2" type="LightWall" x="0" y="0.499995" width="16" height="1.27274"/>
   <object id="3" type="Collision" x="0" y="-9.63636" width="17.2727" height="19.7273"/>
  </objectgroup>
 </tile>
 <tile id="771">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-4.90909" y="-8.27273" width="11.6591" height="14.2727"/>
   <object id="2" type="LightWall" x="0.409091" y="0.136359" width="3" height="3.9091"/>
   <object id="3" type="Coll" x="8.90909" y="5.63636" width="0.0909091">
    <ellipse/>
   </object>
   <object id="4" type="Collision" x="0.181818" y="3.63636" width="6.63636" height="6.90909">
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="772">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-5" y="-5.54545" width="11.75" height="21.5455"/>
   <object id="2" type="Collision" x="6.90909" y="-5.54545" width="9.09091" height="15.5455"/>
   <object id="3" type="LightWall" x="1.68182" y="1.22727" width="1.27273" height="14.8182"/>
   <object id="4" type="LightWall" x="1.5" y="1.13636" width="14.3636" height="1.27273"/>
  </objectgroup>
 </tile>
 <tile id="774">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="1.27273" y="0.181818" width="14.1818" height="5.36364"/>
   <object id="2" type="LightWall" x="1.27273" y="0.261359" width="14.1818" height="2.49175"/>
   <object id="3" type="Collision" x="0.863636" y="-4.04545" width="14.8182" height="14.4545">
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="834">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="12.75" y="13.2216" width="3" height="3"/>
   <object id="2" type="LightWall" x="11.75" y="11.9034" width="4" height="4.18182"/>
  </objectgroup>
 </tile>
 <tile id="835">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.272727" y="13.0227" width="16" height="10.0909"/>
   <object id="2" type="LightWall" x="-0.272727" y="14.7955" width="16" height="2.18183"/>
  </objectgroup>
 </tile>
 <tile id="836">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.272727" y="12.8409" width="16" height="11.4545"/>
   <object id="2" type="LightWall" x="-0.272727" y="14.8864" width="16" height="2.18183"/>
  </objectgroup>
 </tile>
 <tile id="837">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="0.0454545" y="12.8409" width="3" height="3"/>
   <object id="2" type="LightWall" x="0.0454545" y="12.1591" width="2.81818" height="3.54546"/>
  </objectgroup>
 </tile>
 <tile id="866">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="9.0625" y="-0.0227295" width="12.0227" height="16"/>
   <object id="2" type="LightWall" x="12.6193" y="0.113629" width="1.54545" height="15.9091"/>
  </objectgroup>
 </tile>
 <tile id="869">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-3.46591" y="-0.0227295" width="10.4773" height="16"/>
   <object id="2" type="LightWall" x="0.488637" y="0.113629" width="2.27273" height="15.9091"/>
  </objectgroup>
 </tile>
 <tile id="870">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="8.86363" y="-0.0454506" width="8.45455" height="18.5455"/>
   <object id="2" type="Collision" x="-0.0454643" y="9.95455" width="8.90909" height="8.27273"/>
   <object id="3" type="LightWall" x="-0.136354" y="11.7727" width="11.6363" height="1.27273"/>
   <object id="4" type="LightWall" x="12.6818" y="-0.318188" width="1.27273" height="8.8182"/>
   <object id="5" type="LightWall" x="9.2273" y="8.22728" width="4.09085" height="3.54546"/>
  </objectgroup>
 </tile>
 <tile id="897">
  <objectgroup draworder="index" id="2">
   <object id="1" type="PuzzleGridRoad" x="11" y="10">
    <point/>
   </object>
  </objectgroup>
 </tile>
 <tile id="898">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="9.25" y="0.125" width="11.2955" height="16"/>
   <object id="2" type="LightWall" x="13.9432" y="0.136359" width="1.36364" height="15.9091"/>
  </objectgroup>
 </tile>
 <tile id="901">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-3.73864" y="0" width="10.4773" height="16"/>
   <object id="2" type="LightWall" x="0.0795455" y="0" width="2.36364" height="15.9091"/>
  </objectgroup>
 </tile>
 <tile id="902">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-2.90909" y="0.227277" width="9.90909" height="17.5455"/>
   <object id="2" type="Collision" x="7.09091" y="13.1364" width="8.72727" height="4.72727"/>
   <object id="3" type="LightWall" x="1.72727" y="0.136359" width="1.27273" height="13.4546"/>
   <object id="4" type="LightWall" x="1.72729" y="12.3182" width="14" height="1.27273"/>
  </objectgroup>
 </tile>
 <tile id="905">
  <objectgroup draworder="index" id="2">
   <object id="2" type="Pit" x="13.0625" y="11.0227" width="2.76" height="5"/>
  </objectgroup>
 </tile>
 <tile id="906">
  <objectgroup draworder="index" id="2">
   <object id="2" type="Pit" x="0.0624909" y="10.9659" width="15.8182" height="5"/>
  </objectgroup>
 </tile>
 <tile id="907">
  <objectgroup draworder="index" id="2">
   <object id="2" type="Pit" x="-0.0341" y="10.9943" width="2.76" height="5"/>
  </objectgroup>
 </tile>
 <tile id="930">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="9.17617" y="0.125" width="9.38636" height="10"/>
   <object id="2" type="LightWall" x="13.3125" y="0.29545" width="2.63636" height="2.54546"/>
  </objectgroup>
 </tile>
 <tile id="931">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-0.181818" y="-5.81818" width="16" height="15.3636"/>
   <object id="2" type="LightWall" x="-0.181818" y="1.04545" width="16" height="1.27274"/>
  </objectgroup>
 </tile>
 <tile id="932">
  <objectgroup draworder="index" id="3">
   <object id="3" type="Collision" x="-0.206522" y="-6.04842" width="16" height="15.3636"/>
   <object id="4" type="LightWall" x="-0.206522" y="0.815212" width="16" height="1.27274"/>
  </objectgroup>
 </tile>
 <tile id="933">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-3.46591" y="-4.65909" width="10.2955" height="13"/>
   <object id="2" type="LightWall" x="0.488636" y="0.477266" width="3" height="3.9091"/>
  </objectgroup>
 </tile>
 <tile id="934">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Collision" x="-1.81818" y="-1.93183" width="8.56818" height="17.7273"/>
   <object id="2" type="Collision" x="6.90909" y="-1.93183" width="9.09091" height="11.7273"/>
   <object id="3" type="LightWall" x="1.68182" y="1.02272" width="1.27273" height="14.8182"/>
   <object id="4" type="LightWall" x="1.5" y="0.931807" width="14.3636" height="1.27273"/>
  </objectgroup>
 </tile>
 <tile id="937">
  <objectgroup draworder="index" id="2">
   <object id="2" type="Pit" x="13.3352" y="0.0909" width="2.7557" height="15.8182"/>
  </objectgroup>
 </tile>
 <tile id="938">
  <objectgroup draworder="index" id="2">
   <object id="3" type="Pit" x="0.0909" y="-0.0454555" width="15.9091" height="16"/>
  </objectgroup>
 </tile>
 <tile id="939">
  <objectgroup draworder="index" id="2">
   <object id="2" type="Pit" x="0.0909" y="0.227272" width="2.76" height="15.8182"/>
  </objectgroup>
 </tile>
 <tile id="969">
  <objectgroup draworder="index" id="2">
   <object id="2" type="Pit" x="13.1534" y="0.0454536" width="2.76" height="5"/>
  </objectgroup>
 </tile>
 <tile id="970">
  <objectgroup draworder="index" id="2">
   <object id="1" type="Pit" x="0.0909091" y="0.0909091" width="15.8182" height="5"/>
  </objectgroup>
 </tile>
 <tile id="971">
  <objectgroup draworder="index" id="2">
   <object id="2" type="Pit" x="0.0909" y="0.227272" width="2.76" height="5"/>
  </objectgroup>
 </tile>
</tileset>
