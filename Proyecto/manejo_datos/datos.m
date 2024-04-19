acel = readtable("../Datos_finales/medida1_ace.csv");
gyro = readtable("../Datos_finales/medida1_gyro.csv");

%%
figure(1)
tiledlayout(2,1)

nexttile
hold on
title("Acelerometer")
plot(-1*acel.x);    % eje x
plot(-1*acel.z);    % eje y
plot(acel.y);       % eje z
legend('x', 'y', 'z')
axis([0 2.2e4 -1.5 0.5])

nexttile
hold on
title("Gyroscope")
plot(gyro.x);       % eje x 
plot(gyro.z);       % eje y 
plot(-1*gyro.y);    % eje z
legend('x', 'y', 'z')
axis([0 2.2e4 -60 80])

%% 3d accelerometer
% figure(2)
% % set(gca,'CameraPosition',[1 2 3]);
% 
% TIEMPO = size(acel.x);
% point = [0 0 0];
% 
% for t = 1:TIEMPO
%     figure(2)
%     tiledlayout(1,2)
%     nexttile
%     point = [acel.x(t) acel.y(t) acel.z(t)];
%     plot3([0 point(1)], [0 point(2)], [0 point(3)]);
%     xlim([-1 1]);
%     ylim([-1 1]);
%     zlim([-1 1]);
%     grid on
% 
%     nexttile
%     hold on
%     title("Acelerometer")
%     plot(acel.x);
%     plot(acel.y);
%     plot(acel.z);
%     ylim([-2 2]);
%     xlim([t-100, t]);
%     grid on
% 
%     drawnow
%     % pause(0.002);
% end
