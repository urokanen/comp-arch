module stack_behaviour_normal(io_data, reset, clk, command, index);
    inout wire[3:0] io_data;
    input wire[2:0] index;
    input wire[1:0] command;
    input wire clk, reset;
    reg [3:0] mas[4:0];
    integer i;
    reg[3:0] ind, ind2, get, res;
    reg rw;
    
    always @ (posedge reset) begin
        ind = 0;
        ind2 = 4;
        rw = 0;
        res = 0;
        for (i = 0; i < 5; i+=1) begin
            mas[i] = 4'b0000;
        end
    end
    
    always @ (posedge clk or negedge reset) begin
        if (clk == 1 & reset == 0) begin
            if (command == 2'b01) begin
                rw = 0;
                for (i = 0; i < 5; i += 1) begin
                    if (ind == i) begin
                        mas[i] = io_data;
                    end
                end
                ind2 += 1;
                ind += 1;
                ind %= 5;
                ind2 %= 5;
            end else if (command == 2'b10) begin
                rw = 1;
                res = mas[ind2];
                ind2 -= 1;
                ind -= 1;
                ind %= 5;
                ind2 %= 5;
            end else if (command == 2'b11) begin
                rw = 1;
                get = ind2 - (index % 5);
                if (get > 4) begin
                    get = get + 5;
                end
                res = mas[get];
            end 
        end
    end
    
    assign io_data = (rw) ? res : 4'bZZZZ;
endmodule
