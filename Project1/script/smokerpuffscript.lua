function SmokerPuff_init ()
	_E = { x=0, y=0, w=0, h=0, tox=-12, toy=12, tw=24, th=24, u=72, v=0, uvw=24, uvh=24 }
	_D = { "kill", "0", "frame", "0", "ushift", "72", "velx", "0", "vely", "0" }
	--		 0             1              2            3             4
	_T = 5
end

function SmokerPuff ()
	SmokerPuff_anim()
	if (getdatai(this, 1) == 12) then
		setdatai(this, 0, 2)
	end
	pos = getpos(this)
	vel_x = getdatai(this, 3)
	vel_y = getdatai(this, 4)
	frame = getdatai(this, 1)
	setpos(this, pos.x + vel_x, pos.y + vel_y)
	if (vel_x ~= 0 and frame % 3 == 0) then
		vel_x = 0
	end
	if (vel_y ~= 0 and frame % 3 == 0) then
		vel_y = 0
	end
	setdatai(this, 3, vel_x)
	setdatai(this, 4, vel_y)
	setdatai(this, 1, getdatai(this, 1) + 1)
end

function SmokerPuff_anim()
	if ((math.floor(getdatai(this, 1)) % 4 == 0) and (getdatai(this, 1) ~= 0)) then
		setdatai(this, 2, getdatai(this, 2) + 24)
	end
	setuv(this, getdatai(this, 2), getuv(this).v)
end