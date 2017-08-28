@echo off
SET CPY=xcopy /d /y /i /s
%CPY% src\client\client.cfg bin\
%CPY% src\server\server.cfg bin\

%CPY% External\dll\*.dll bin\

%CPY% server_data bin\server_data

%CPY% client_data bin\client_data