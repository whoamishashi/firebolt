<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="16008000">
	<Property Name="varPersistentID:{09590C38-0D28-4663-95E1-8BCB80CD07B1}" Type="Ref">/My Computer/Controls/library.lvlib/pagedown</Property>
	<Property Name="varPersistentID:{13A60434-9823-412B-96A2-3A6D9AD338B3}" Type="Ref">/My Computer/Controls/library.lvlib/up</Property>
	<Property Name="varPersistentID:{227F57DE-AEC7-493E-B406-9BC01E4906DD}" Type="Ref">/My Computer/Controls/library.lvlib/right</Property>
	<Property Name="varPersistentID:{291D52FD-47ED-4823-9BD6-EC42FD7181C3}" Type="Ref">/My Computer/Controls/library.lvlib/pageup</Property>
	<Property Name="varPersistentID:{87253833-0CF7-4AF2-81EE-A672383685D7}" Type="Ref">/My Computer/Controls/library.lvlib/left</Property>
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="Controls" Type="Folder">
			<Item Name="compass.ctl" Type="VI" URL="../compass.ctl"/>
			<Item Name="library.lvlib" Type="Library" URL="../library.lvlib"/>
		</Item>
		<Item Name="Control 1.ctl" Type="VI" URL="../Control 1.ctl"/>
		<Item Name="Ground_station.vi" Type="VI" URL="../Ground_station.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
				<Item Name="Space Constant.vi" Type="VI" URL="/&lt;vilib&gt;/dlg_ctls.llb/Space Constant.vi"/>
				<Item Name="VISA Configure Serial Port" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port"/>
				<Item Name="VISA Configure Serial Port (Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Instr).vi"/>
				<Item Name="VISA Configure Serial Port (Serial Instr).vi" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Configure Serial Port (Serial Instr).vi"/>
			</Item>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
