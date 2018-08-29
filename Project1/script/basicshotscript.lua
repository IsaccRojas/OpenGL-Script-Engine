function BasicShot_init ()
	_E = { x=0, y=0, w=0, h=0, tox=-8, toy=8, tw=16, th=16, u=0, v=0, uvw=16, uvh=16 }
	_D = { "kill", "0", "time", "0", "shift", "0", "velx", "0", "vely", "0", "collided", "0", "dmg", "5" }
	--       0             1             2            3           4               5             6
	_T = 4
	math.randomseed( os.time() )
	math.random(); math.random(); math.random()
end

function BasicShot ()
	BasicShot_anim()
	pos = getpos(this)
	setpos(this, pos.x + getdatai(this, 3), pos.y + getdatai(this, 4))
	if ((getdatai(this, 1) == 75) or getdatai(this, 5) > 0) then
		setdatai(this, 0, 2)
	end
	setdatai(this, 1, getdatai(this, 1) + 1)
end

function BasicShot_anim()
	if (getdatai(this, 1) % 3 == 0) then
		xshift = math.random(-2, 2)
		yshift = math.random(-2, 2)
		Ef = gen_ent(master, "BasicShotPuff")
		mepos = getpos(this)
		setpos(Ef, mepos.x + xshift, mepos.y + yshift)
	end
end