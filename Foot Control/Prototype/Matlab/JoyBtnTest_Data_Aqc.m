clear all

a = arduino;

% Initialize
joy = zeros(500,1);
joyBtn = zeros(500,1);
btn = zeros(500,1);
t = seconds(joy);
configurePin(a,'D2','DigitalInput')
configurePin(a,'D4','Pullup')

t0 = datetime('now');
for ii = 1:500
    joy(ii) = readVoltage(a,'A1');
    joyBtn(ii) = readDigitalPin(a,'D2')*4.975;
    btn(ii) = readDigitalPin(a,'D4');
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
