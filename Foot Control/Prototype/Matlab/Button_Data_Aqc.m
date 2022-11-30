a = arduino('COM3','Uno'); % To call in arduino, must have same port #

input = zeros(500,1);
t = seconds(input);
%%

t0 = datetime('now');
for ii = 1:500
    input(ii) = readDigitalPin(a,'D2');
    t(ii) = datetime('now') - t0;
end 

plot(t,input);
%%