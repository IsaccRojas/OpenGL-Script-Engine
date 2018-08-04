function Effect_init ()
	_E = { x=0, y=0, w=0, h=0, tox=-24, toy=24, tw=48, th=48, u=0, v=0, uvw=48, uvh=48 }
	_D = { "kill", "0", "frame", "0", "ushift", "0" }
	_F0 = { tox=0, toy=2, tw=4, th=6, u=8, v=10, uvw=12, uvh=14 }
	_T = 4
end

function Effect ()
	Effect_anim()
	if (getdatai(this, 1) == 20) then
		setdatai(this, 0, 2)
	end
	setdatai(this, 1, getdatai(this, 1) + 1)
end

function Effect_anim()
	if ((math.floor(getdatai(this, 1)) % 4 == 0) and (getdatai(this, 1) ~= 0)) then
		setdatai(this, 2, getdatai(this, 2) + 48)
	end
	setuv(this, getdatai(this, 2), 0)
end