function DamageText_init ()
	_E = { x=0, y=0, w=0, h=0, tox=-2, toy=2.5, tw=4, th=5, u=0, v=13, uvw=4, uvh=5 }
	_D = { "kill", "0", "spacing", "1", "reset", "0" }
	--		 0              1			   2
	_T = 6
end

function DamageText ()
	--[[
	if (getdatai(this, 2) == 1) then
		Text_set("Hello!")
		setdatai(this, 2, 0)
	end
	]]
end

function DamageText_set(str)
	
end