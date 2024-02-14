acel = readtable("acel.csv");
gyro = readtable("gyro.csv");

%%
figure(1)
tiledlayout(2,1)

nexttile
hold on
title("Acelerometer")
plot(acel.x);
plot(acel.y);
plot(acel.z);
legend('x', 'y', 'z')

nexttile
hold on
title("Gyroscope")
plot(gyro.x);
plot(gyro.y);
plot(gyro.z);
legend('x', 'y', 'z')

%% 3d accelerometer
figure(2)
% set(gca,'CameraPosition',[1 2 3]);

TIEMPO = size(acel.x);
point = [0 0 0];

for t = 1:TIEMPO
    figure(2)
    tiledlayout(1,2)
    nexttile
    point = [acel.x(t) acel.y(t) acel.z(t)];
    plot3([0 point(1)], [0 point(2)], [0 point(3)]);
    xlim([-1 1]);
    ylim([-1 1]);
    zlim([-1 1]);
    grid on
    
    nexttile
    hold on
    title("Acelerometer")
    plot(acel.x);
    plot(acel.y);
    plot(acel.z);
    ylim([-2 2]);
    xlim([t-100, t]);
    grid on

    drawnow
    % pause(0.002);
end
