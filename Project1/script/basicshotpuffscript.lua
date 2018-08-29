function BasicShotPuff_init ()
	_E = { x=0, y=0, w=0, h=0, tox=-4, toy=4, tw=8, th=8, u=0, v=0, uvw=8, uvh=8 }
	_D = { "kill", "0", "frame", "0", "ushift", "0" }
	_T = 5
end

function BasicShotPuff ()
	BasicShotPuff_anim()
	if (getdatai(this, 1) == 12) then
		setdatai(this, 0, 2)
	end
	setdatai(this, 1, getdatai(this, 1) + 1)
end

function BasicShotPuff_anim()
	if ((math.floor(getdatai(this, 1)) % 4 == 0) and (getdatai(this, 1) ~= 0)) then
		setdatai(this, 2, getdatai(this, 2) + 8)
	end
	setuv(this, getdatai(this, 2), getuv(this).v)
end