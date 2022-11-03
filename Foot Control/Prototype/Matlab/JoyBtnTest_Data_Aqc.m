clear all

a = arduino;

joy = zeros(500,1);
joyBtn = zeros(500,1);
t = seconds(joy);
%%

t0 = datetime('now');
for ii = 1:500
    joy(ii) = readVoltage(a,'A1');
    %% joyBtn(ii) = readDigitalPin(a,'D2')*4.975;
    t(ii) = datetime('now') - t0;
end 

plot(t,joy);
%% scatter(t,joyBtn);
%
% Joystick neutral is around 4.9805 - 4.9775
