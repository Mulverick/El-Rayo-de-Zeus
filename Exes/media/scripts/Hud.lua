-- Fichero Hud.lua

-- Vars initialization showed in HUD

numberAltarsActivated = 0
numberEnemiesInBase = 0
PM = 0
lifeBase = lifeBaseMax

--Funcion de inicializacion/carga de un layout y sus ventanas y subventanas
function initHud()

	if hud==nil then
	
		--GUI.Windows:loadWindowLayout("Hud.layout")		
		
		root = GUI.Windows:getWindow("Root")
		
		--hud = GUI.Windows:getWindow("Hud")
		hud = GUI.Windows:getWindow("Root/Hud")
		
		--LifeWindow = hud:getChild("Hud/Life")
		LifeWindow = hud:getChild("Root/Hud/Life")
		
		SwordWindow = hud:getChild("Root/Hud/SwordLifeContainer")
		
		--TitleEnemigosWindow = hud:getChild("Hud/TitleEnemigos")
		--NumEnemigosWindow = hud:getChild("Hud/NumEnemigos")
		
		TitleEnemigosWindow = hud:getChild("Root/Hud/TitleEnemigos")
		NumEnemigosWindow = hud:getChild("Root/Hud/NumEnemigos")		
		
		--TitlePuntosMeritoWindow = hud:getChild("Hud/TitlePuntosMerito")
		--PuntosMeritoWindow = hud:getChild("Hud/PuntosMerito")
		
		TitlePuntosMeritoWindow = hud:getChild("Root/Hud/TitlePuntosMerito")
		PuntosMeritoWindow = hud:getChild("Root/Hud/PuntosMerito")		
		
		--AltarWindow = hud:getChild("Hud/Altares")		
		--NumberAltaresActivatedWindow = hud:getChild("Hud/NumAltares")
		
		AltarWindow = hud:getChild("Root/Hud/Altares")		
		NumberAltaresActivatedWindow = hud:getChild("Root/Hud/NumAltares")		
		
		--PersonajeWindow = hud:getChild("Hud/Personaje")
		
		--PlayernameWindow = hud:getChild("Hud/PlayerName")
		
		--RayoWindow = hud:getChild("Hud/RayoBase")
		
		PersonajeWindow = hud:getChild("Root/Hud/Personaje")
		
		PlayernameWindow = hud:getChild("Root/Hud/PlayerName")
		
		RayoWindow = hud:getChild("Root/Hud/RayoBase")		
	end
end


--Funcion que muestra la ventana del HUD
function showHud()
	--if hud ~= nil then
	if root ~= nil then
		--GUI.System:setGUISheet(hud)
		GUI.System:setGUISheet(root)
		hud:setVisible(true)
		hud:activate()
	end
end


--Funcion que oculta una ventana del GUI
function hideHud()
	--if hud ~= nil then
	if root ~= nil then
		hud:deactivate()		
		hud:setVisible(false)
	end
end


function changeHudVisibility()
		if hud:isVisible() then
			hud:setVisible(false)
		else
			hud:setVisible(true)
		end
end


--Altars HUD functions
function incrementAltarsActivated()
  numberAltarsActivated = numberAltarsActivated+1
  --Control de rango
  if numberAltarsActivated > numberAltarsMax then
    numberAltarsActivated = numberAltarsMax
  end  
end

function decrementAltarsActivated()
  numberAltarsActivated = numberAltarsActivated-1
  --Control de rango  
  if numberAltarsActivated < 0 then
    numberAltarsActivated = 0
  end
end

function updateAltarsActivated()
   NumberAltaresActivatedWindow:setText(numberAltarsActivated..'/'..numberAltarsMax)
end

function showAltarsActivated(altarsactivated, altarsmax)
   NumberAltaresActivatedWindow:setText(altarsactivated..'/'..altarsmax)
end

--NumberEnemies HUD functions
function incrementEnemiesInBase()
  numberEnemiesInBase = numberEnemiesInBase+1
    --Control de rango  
	if numberEnemiesInBase > numberEnemiesInBaseMax then
		numberEnemiesInBase = numberEnemiesInBaseMax
	end  
end

function decrementEnemiesInBase()
  numberEnemiesInBase = numberEnemiesInBase-1
    --Control de rango
	if numberEnemiesInBase < 0 then
		numberEnemiesInBase = 0
	end
end

function updateEnemiesInBase()
   NumEnemigosWindow:setText(numberEnemiesInBase)
end

function showEnemiesInBase(enemies)
   NumEnemigosWindow:setText(enemies)
end

-- pM HUD functions
function incrementPM()
  PM = PM+incPM
end

function decrementPM()
  PM = PM-incPM
end

function updatePM()
   PuntosMeritoWindow:setText(PM)
end

function showPM(pm)
   PuntosMeritoWindow:setText(pm)
end

-- lifeBase HUD functions
function incrementBaseLife()
  lifeBase = lifeBase+1
  --Control de rango
	if lifeBase > lifeBaseMax then
		lifeBase = lifeBaseMax
	end	  
end

function decrementBaseLife()
  lifeBase = lifeBase-1
  RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeDamage")
  --Control de rango
  if lifeBase < 0 then
	lifeBase = 0
  end  
end


function updateBaseLife()
	if lifeBase == 3 then
	   RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeEntire")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoActivado")
    elseif lifeBase == 2 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeAlmostEntire")	
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 1 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeAlmostEmpty")	
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 0 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeEmpty")	
      --Rayo1Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    end
end

function showBaseLife()
	if lifeBase == 3 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeEntire")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoActivado")
    elseif lifeBase == 2 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeAlmostEntire")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 1 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeAlmostEmpty")
	  --Rayo1Window:setProperty("Image","set:HudBackground image:RayoActivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    elseif lifeBase == 0 then
	    RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeEmpty")
      --Rayo1Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo2Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
	  --Rayo3Window:setProperty("Image","set:HudBackground image:RayoDesactivado")
    end
end

function reloadHud()
		-- hud:setProperty("UnifiedAreaRect","{{0,0},{0,0},{0,504},{0,193}}")
		hud:setProperty("UnifiedAreaRect","{{0,0},{0,0},{0,300},{0,150}}")
		PersonajeWindow:setProperty("UnifiedAreaRect","{{0,35},{0,15},{0,115},{0,95}}")
		RayoWindow:setProperty("Image","set:HudBaseLife image:BaseLifeDamage")
		RayoWindow:setProperty("UnifiedAreaRect","{{0.0,230},{0.0,40},{0.0,290},{0.0,100}}")
		
		-- PlayernameWindow:setProperty("UnifiedAreaRect","{{0.5,0},{0.15,0},{0.9,0},{0.25,0}}")
		PlayernameWindow:setProperty("UnifiedAreaRect","{{0,110},{0,5},{0,290},{0,25}}")
		PlayernameWindow:setProperty("Font","Cantoria-14")
		
		--TitleEnemigosWindow:setProperty("UnifiedAreaRect","{{0.5,0},{0.28,0},{0.9,0},{0.48,0}}")
		TitleEnemigosWindow:setProperty("UnifiedAreaRect","{{0,120},{0,26},{0,215},{0,48}}")
		TitleEnemigosWindow:setProperty("Font","Cantoria-8")
		--NumEnemigosWindow:setProperty("UnifiedAreaRect","{{0.705,0},{0.28,0},{0.74,0},{0.48,0}}")
		NumEnemigosWindow:setProperty("UnifiedAreaRect","{{0,220},{0,26},{0,230},{0,48}}")
		NumEnemigosWindow:setText("1234567")
		NumEnemigosWindow:setProperty("Font","Cantoria-8")
		
		--TitlePuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0.4,0},{0.5,0},{0.55,0},{0.7,0}}")
		TitlePuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0,125},{0,50},{0,170},{0,72}}")
		TitlePuntosMeritoWindow:setProperty("Font","Cantoria-8")
		--PuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0.56,0},{0.5,0},{0.68,0},{0.7,0}}")
		PuntosMeritoWindow:setProperty("Font","Cantoria-8")
		PuntosMeritoWindow:setProperty("UnifiedAreaRect","{{0,165},{0,50},{0,230},{0,72}}")
		PuntosMeritoWindow:setText("12324556679")
		
		AltarWindow:setProperty("UnifiedAreaRect","{{0,115},{0,74},{0,205},{0,96}}")
		AltarWindow:setProperty("Font","Cantoria-8")
		NumberAltaresActivatedWindow:setProperty("UnifiedAreaRect","{{0,180},{0,74},{0,230},{0,96}}")
		NumberAltaresActivatedWindow:setProperty("Font","Cantoria-8")	
		NumberAltaresActivatedWindow:setText("11/11")		
		
		-- Vida del jugador embebida en una espada o similar
		LifeWindow:setProperty("UnifiedAreaRect","{{0,58},{0,98},{0,215},{0,111}}")
		SwordWindow:setProperty("UnifiedAreaRect","{{0,1},{0,95},{0,215},{0,142}}")
		
end

function initNickName(nickname)
	PlayernameWindow:setText(nickname)
end

function updateHudLife(ratio)
	local pbar = CEGUI.toProgressBar(LifeWindow)
	pbar:setProgress(ratio)
end

