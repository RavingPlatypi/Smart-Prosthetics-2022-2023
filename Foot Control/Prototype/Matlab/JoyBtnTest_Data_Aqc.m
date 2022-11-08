clear all

a = arduino;

joy = zeros(500,1);
joyBtn = zeros(500,1);
btn = zeros(500,1);
t = seconds(joy);


t0 = datetime('now');
for ii = 1:500
    joy(ii) = readVoltage(a,'A1');
    joyBtn(ii) = readDigitalPin(a,'D2')*4.975;
    btn(ii) = readDigitalPin(a,'D3');
    t(ii) = datetime('now') - t0;
end 

figure(1);
plot(t,joy);
figure(2);
plot(t,joyBtn);
figure(3);
plot(t,btn);
%
% Joystick neutral is around 2.5
