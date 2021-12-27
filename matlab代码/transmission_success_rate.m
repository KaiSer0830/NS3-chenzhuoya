clear all;              %%清除工作空间的所有变量
all_result1 = [];
all_nodeNums = [5; 10; 15; 20; 25; 30];
for i=1:6
    file11 = ['D:\professional_install\matlab\bin\project_files\router_flooding\flooding_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid11 = fopen(file11);
    C11 = textscan(fid11, '%f%f%f%f%f');          %%其中fid为fopen命令返回的文件标识符，format实际上就是一个字符串变量，表示读取数据及数据转换的规则。
    fclose(fid11);
    dataRx1 = [C11{5}];                         %%取出第5个元素
    file12 = ['D:\professional_install\matlab\bin\project_files\router_flooding\flooding_RES_TX_', num2str(all_nodeNums(i,:)), '.log'];
    fid12 = fopen(file12);
    C12 = textscan(fid12, '%f%f');          %%其中fid为fopen命令返回的文件标识符，format实际上就是一个字符串变量，表示读取数据及数据转换的规则。
    fclose(fid12);
    dataTx1 = [C12{2}];                         %%取出第5个元素
    result = length(dataRx1) / length(dataTx1);
    all_result1 = [all_result1; result];
end
plot(all_nodeNums(:,1), all_result1(:,1), '-->g');
ylabel('Transmission success rate(%)');
xlabel('Density of Nodes');

hold on
all_result2 = [];
for i=1:6
    file21 = ['D:\professional_install\matlab\bin\project_files\router_random\random_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid21 = fopen(file21);
    C21 = textscan(fid21, '%f%f%f%f%f');          %%其中fid为fopen命令返回的文件标识符，format实际上就是一个字符串变量，表示读取数据及数据转换的规则。
    fclose(fid21);
    dataRx2 = [C21{5}];                         %%取出第5个元素
    file22 = ['D:\professional_install\matlab\bin\project_files\router_random\random_RES_TX_', num2str(all_nodeNums(i,:)), '.log'];
    fid22 = fopen(file22);
    C22 = textscan(fid22, '%f%f');          %%其中fid为fopen命令返回的文件标识符，format实际上就是一个字符串变量，表示读取数据及数据转换的规则。
    fclose(fid22);
    dataTx2 = [C22{2}];                         %%取出第5个元素
    result = length(dataRx2) / length(dataTx2);
    all_result2 = [all_result2; result];
end
plot(all_nodeNums(:,1), all_result2(:,1), '--db');

hold on
all_result3 = [];
for i=1:6
    file31 = ['D:\professional_install\matlab\bin\project_files\router_opport\opport_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid31 = fopen(file31);
    C31 = textscan(fid31, '%f%f%f%f%f');          %%其中fid为fopen命令返回的文件标识符，format实际上就是一个字符串变量，表示读取数据及数据转换的规则。
    fclose(fid31);
    dataRx3 = [C31{5}];                         %%取出第5个元素
    file32 = ['D:\professional_install\matlab\bin\project_files\router_opport\opport_RES_TX_', num2str(all_nodeNums(i,:)), '.log'];
    fid32 = fopen(file32);
    C32 = textscan(fid32, '%f%f');          %%其中fid为fopen命令返回的文件标识符，format实际上就是一个字符串变量，表示读取数据及数据转换的规则。
    fclose(fid32);
    dataTx3 = [C32{2}];                         %%取出第5个元素
    result = length(dataRx3) / length(dataTx3);
    all_result3 = [all_result3; result];
end
plot(all_nodeNums(:,1), all_result3(:,1), '--pr');
legend('Flooding', 'Random', 'Opport');




