data  = load('data/data');
t     = data(:, 1);
temp  = data(:, 2);
humid = data(:, 3);
rssi  = abs(data(:, 4));

figure; hold on;
ylim([0, 100]);

t = t./(60*60); % seconds in an hour

plot(t, temp,  'r', 'displayname', 'temperature');
plot(t, humid, 'b', 'displayname', 'humidity');
plot(t, rssi,  'g', 'displayname', 'rssi');
legend('show');

pause
