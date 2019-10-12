function BasicShotExplode_init ()
	_E = { x=0, y=0, w=0, h=0, tox=-8, toy=8, tw=16, th=16, u=24, v=0, uvw=16, uvh=16 }
	_D = { "kill", "0", "frame", "0", "ushift", "0" }
	_T = 5
end

function BasicShotExplode ()
	BasicShotExplode_anim()
	if (getdatai(this, 1) == 12) then
		setdatai(this, 0, 2)
	end
	setdatai(this, 1, getdatai(this, 1) + 1)
end

function BasicShotExplode_anim()
	if ((math.floor(getdatai(this, 1)) % 4 == 0) and (getdatai(this, 1) ~= 0)) then
		setdatai(this, 2, getdatai(this, 2) + 16)
	end
	setuv(this, 24 + getdatai(this, 2), getuv(this).v)
end