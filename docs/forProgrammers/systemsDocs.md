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
<td>AnimationSystem</td>
<td>
</td>
<td>
AnimationData<br />
TextureRect<br />
</td>
</tr>
<tr>
<td>AudioSystem</td>
<td>
Player<br />
BodyRect<br />
Damage<br />
</td>
<td>
AmbientSound<br />
SpatialSound<br />
</td>
</tr>
<tr>
<td>CanUseWeapon</td>
<td>
CurrentMeleeWeapon<br />
MeleeProperties<br />
CurrentGun<br />
GunProperties<br />
</td>
<td>
MeleeAttacker<br />
GunAttacker<br />
</td>
</tr>
<tr>
<td>CollisionDebug</td>
<td>
StaticCollisionBody<br />
BodyRect<br />
MultiStaticCollisionBody<br />
KinematicCollisionBody<br />
</td>
<td>
</td>
</tr>
<tr>
<td>DamageAndDeath</td>
<td>
</td>
<td>
DamageTag<br />
Health<br />
DamageAnimation<br />
RenderQuad<br />
MultiParticleEmitter<br />
</td>
</tr>
<tr>
<td>EntityDestroying</td>
<td>
</td>
<td>
ALL<br />
</td>
</tr>
<tr>
<td>Entrances</td>
<td>
Player<br />
Entrance<br />
BodyRect<br />
</td>
<td>
</td>
</tr>
<tr>
<td>GameplayUI</td>
<td>
Player<br />
GunAttacker<br />
Health<br />
</td>
<td>
</td>
</tr>
<tr>
<td>Gates</td>
<td>
</td>
<td>
Gate<br />
LeverListener<br />
StaticCollisionBody<br />
BlocksLight<br />
HiddenForRenderer<br />
</td>
</tr>
<tr>
<td>GunAttacks</td>
<td>
Player<br />
FaceDirection<br />
BodyRect<br />
Killable<br />
</td>
<td>
CurrentGun<br />
GunAttacker<br />
HiddenForRenderer<br />
InPlayerAttackArea<br />
DamageTag<br />
AmbientSound<br />
Lifetime<br />
LastingShot<br />
</td>
</tr>
<tr>
<td>GunPositioningAndTexture</td>
<td>
Player<br />
CurrentGun<br />
FaceDirection<br />
</td>
<td>
TextureRect<br />
HiddenForRenderer<br />
BodyRect<br />
GunAttacker<br />
</td>
</tr>
<tr>
<td>HostileCollisions</td>
<td>
Player<br />
BodyRect<br />
Damage<br />
Health<br />
</td>
<td>
CollisionWithPlayer<br />
DamageTag<br />
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
<tr>
<td>KinematicCollisions</td>
<td>
KinematicCollisionBody<br />
</td>
<td>
BodyRect<br />
Velocity<br />
</td>
</tr>
<tr>
<td>Levers</td>
<td>
Player<br />
BodyRect<br />
</td>
<td>
Lever<br />
TextureRect<br />
LeverListener<br />
</td>
</tr>
<tr>
<td>Lifetime</td>
<td>
</td>
<td>
TaggedToDestroy<br />
Lifetime<br />
</td>
</tr>
<tr>
<td>MeleeAttacks</td>
<td>
Player<br />
BodyRect<br />
FaceDirection<br />
MeleeProperties<br />
CurrentMeleeWeapon<br />
Killable<br />
</td>
<td>
MeleeAttacker<br />
InPlayerAttackArea<br />
DamageTag<br />
</td>
</tr>
<tr>
<td>MeleePositioning</td>
<td>
Player<br />
FaceDirection<br />
MeleeAttacker<br />
</td>
<td>
BodyRect<br />
HiddenForRenderer<br />
CurrentMeleeWeapon<br />
RenderQuad<br />
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
<td>ParticleSystem</td>
<td>
<br />
</td>
<td>
<br />
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
TaggedToDestroy<br />
</td>
</tr>
<tr>
<td>PlayerCameraMovement</td>
<td>
Player<br />
BodyRect<br />
</td>
<td>
Camera<br />
</td>
</tr>
<tr>
<td>PlayerMovementInput</td>
<td>
Player<br />
TimeToFadeOut<br />
CharacterSpeed<br />
BodyRect<br />
</td>
<td>
AnimationData<br />
FaceDirection<br />
LightSource<br />
GunAttacker<br />
MeleeAttacker<br />
Velocity<br />
</td>
</tr>
<tr>
<td>PushingAreas</td>
<td>
PushingArea<br />
BodyRect<br />
KinematicCollisionBody<br />
</td>
<td>
Velocity<br />
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
<td>StaticCollisions</td>
<td>
StaticCollisionBody<br />
MultiStaticCollisionBody<br />
</td>
<td>
BodyRect<br />
KinematicCollisionBody<br />
</td>
</tr>
<tr>
<td>VelocityChangingAreas</td>
<td>
BodyRect<br />
AreaVelocityChangingEffect<br />
KinematicCollisionBody<br />
</td>
<td>
Velocity<br />
</td>
</tr>
<tr>
<td>VelocityClear</td>
<td>
</td>
<td>
Velocity<br />
</td>
</tr>
<tr>
<td>ZombieSystem</td>
<td>
BodyRect<br />
TimeToFadeOut<br />
</td>
<td>
Zombie<br />
Velocity<br />
SpatialSound<br />
</td>
</tr>
</table>
</body>
</html>
