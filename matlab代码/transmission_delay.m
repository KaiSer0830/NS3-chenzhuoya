clear all;              %%清除工作空间的所有变量
all_result1 = [];
all_nodeNums = [5; 10; 15; 20; 25; 30];
for i=1:6
    file = ['D:\professional_install\matlab\bin\project_files\router_flooding\flooding_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid = fopen(file);
    C = textscan(fid, '%f%f%f%f%f');          %%其中fid为fopen命令返回的文件标识符，format实际上就是一个字符串变量，表示读取数据及数据转换的规则。
    fclose(fid);
    data = [C{5}];                         %%取出第5个元素
    s = sum(data);
    result = s / length(data);
    all_result1 = [all_result1; result];
end
plot(all_nodeNums(:,1), all_result1(:,1), '-->g');
ylabel('Delay(ps)');
xlabel('Density of Nodes');

hold on
all_result2 = [];
for i=1:6
    file = ['D:\professional_install\matlab\bin\project_files\router_random\random_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid = fopen(file);
    C = textscan(fid, '%f%f%f%f%f');          %%其中fid为fopen命令返回的文件标识符，format实际上就是一个字符串变量，表示读取数据及数据转换的规则。
    fclose(fid);
    data = [C{5}];                         %%取出第5个元素
    s = sum(data);
    result = s / length(data);
    all_result2 = [all_result2; result];
end
plot(all_nodeNums(:,1), all_result2(:,1), '--db');

hold on
all_result3 = [];
for i=1:6
    file = ['D:\professional_install\matlab\bin\project_files\router_opport\opport_RES_RX_', num2str(all_nodeNums(i,:)), '.log'];
    fid = fopen(file);
    C = textscan(fid, '%f%f%f%f%f');          %%其中fid为fopen命令返回的文件标识符，format实际上就是一个字符串变量，表示读取数据及数据转换的规则。
    fclose(fid);
    data = [C{5}];                         %%取出第5个元素
    s = sum(data);
    result = s / length(data);
    all_result3 = [all_result3; result];
end
plot(all_nodeNums(:,1), all_result3(:,1), '--pr');
legend('Flooding', 'Random', 'Opport');




