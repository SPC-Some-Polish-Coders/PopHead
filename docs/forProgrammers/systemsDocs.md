<html>
<head>
<link rel="stylesheet" href="style.css">
</head>
<body>
<table>
<tr>
<th>System name</th>
<th>Components Read</th>
<th>Components Modified</th>
</tr>
<tr>
<td>PlayerInput</td>
<td>
CharacterSpeed<br />
Player<br />
</td>
<td>
Velocity<br />
</td>
</tr>
<tr>
<td>Movement</td>
<td>
Velocity<br />
</td>
<td>
BodyRect<br />
</td>
</tr>
<tr>
<td>RenderSystem</td>
<td>
BodyRect<br />
TexturePtr<br />
TextureRect<br />
ShaderPtr<br />
Color<br />
</td>
<td>
</td>
</tr>
<tr>
<td>DyingCharacters</td>
<td>
Health<br />
</td>
<td>
TaggedToDestroy<br />
</td>
</tr>
<tr>
<td>EntityDestroying</td>
<td>
TaggedToDestroy<br />
</td>
<td>
</td>
</tr>
<tr>
<td>PickupMedkit</td>
<td>
BodyRect<br />
Medkit<br />
Player<br />
</td>
<td>
Health<br />
TaggedToDestroy<br />
</td>
</tr>
<tr>
<td>PickupBullet</td>
<td>
BodyRect<br />
Player<br />
Bullet<br />
</td>
<td>
GunAttacker<br />
</td>
</tr>
<tr>
<td>PlayerAttackType</td>
<td>
Player<br />
</td>
<td>
MelleAtacker<br />
GunAtacker<br />
</td>
</tr>
<tr>
<td>DamageDealing</td>
<td>
Player<br />
BodyRect<br />
Damage<br />
</td>
<td>
CollisionWithPlayer<br />
Health<br />
</td>
</tr>
<tr>
<td>IsPlayerAlive</td>
<td>
Player<br />
Health<br />
</td>
<td>
</td>
</tr>
</table>
</body>
</html>
