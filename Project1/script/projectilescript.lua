function Projectile_init ()
	_E = { x=0, y=0, w=0, h=0, tox=-8, toy=8, tw=16, th=16, u=0, v=0, uvw=32, uvh=32 }
	_D = { "kill", "0", "time", "0", "shift", "0", "velx", "0", "vely", "0", "collided", "0" }
	_T = 1
end

function Projectile ()
	Projectile_anim()
	pos = getpos(this)
	setpos(this, pos.x + getdatai(this, 3), pos.y + getdatai(this, 4))
	if ((getdatai(this, 1) == 25) or getdatai(this, 5) > 0) then
		setdatai(this, 0, 2)
	end
	setdatai(this, 1, getdatai(this, 1) + 1)
end

function Projectile_anim()
	if (getdatai(this, 1) % 5 == 0) then
		toff = gettoff(this)
		tdim = gettdim(this)
		settoff(this, toff.tox + 1, toff.toy - 1)
		settdim(this, tdim.tw - 2, tdim.th - 2)
	end
end