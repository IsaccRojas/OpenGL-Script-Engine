function dist (x1, y1, x2, y2)
	return math.sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1))
end

function Wizard_init ()
	_E = { x=0, y=0, w=32, h=32, tox=-16, toy=16, tw=32, th=32, u=32, v=32, uvw=32, uvh=32 }
	_D = { "kill", "0", "spd", "1", "frame", "0", "bul_spd", "3", "shoot_cooldown", "0", "shoot_cooldown_max", "90" }
	--		 0            1            2              3                  4                          5
	_T = 2
end

function Wizard ()
	
end

function Wizard_anim()
	
end