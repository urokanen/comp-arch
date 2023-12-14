module rs(r, s, reset, q, inv_q);
    input wire r, s, reset;
    output wire q, inv_q;
    wire res;

    or or1(inv_q, reset, res);
    nor nor1(q, r, inv_q);
    nor nor2(res, q, s);
endmodule

module synch_rs(r, s, reset, clk, q, inv_q);
    input wire r, s, reset, clk;
    output wire q, inv_q;
    wire new_r, new_s;

    and and1(new_r, r, clk);
    and and2(new_s, s, clk);
    rs rs1(new_r, new_s, reset, q, inv_q);
endmodule

module d_trigger(d, reset, clk, q, inv_q);
    input wire d, reset, clk;
    output wire q, inv_q;
    wire inv_d;

    not not1(inv_d, d);
    synch_rs synch_rs1(inv_d, d, reset, clk, q, inv_q);
endmodule

module new_and(i_data, key, o_data);
    input wire[3:0] i_data;
    input wire key; 
    output wire[3:0] o_data;

    and and1(o_data[0], i_data[0], key);
    and and2(o_data[1], i_data[1], key);
    and and3(o_data[2], i_data[2], key);
    and and4(o_data[3], i_data[3], key);
endmodule

module result(a0, a1, a2, a3, a4, o_data);
    input wire[3:0] a0, a1, a2, a3, a4;
    output wire[3:0] o_data;

    or_5 or1(a0[0], a1[0], a2[0], a3[0], a4[0], o_data[0]);
    or_5 or2(a0[1], a1[1], a2[1], a3[1], a4[1], o_data[1]);
    or_5 or3(a0[2], a1[2], a2[2], a3[2], a4[2], o_data[2]);
    or_5 or4(a0[3], a1[3], a2[3], a3[3], a4[3], o_data[3]);
endmodule

module and_3(a0, a1, a2, res);
    input wire a0, a1, a2;
    output wire res;
    wire temp;
    
    and and1(temp, a0, a1);
    and and2(res, temp, a2);
endmodule

module check_0(i_data, o_data);
    input wire[2:0] i_data;
    output wire o_data;
    wire[2:0] inv_data;
    
    not not1(inv_data[0], i_data[0]);
    not not2(inv_data[1], i_data[1]);
    not not3(inv_data[2], i_data[2]);
    and_3 and_3_1(inv_data[0], inv_data[1], inv_data[2], o_data);
endmodule

module check_1(i_data, o_data);
    input wire[2:0] i_data;
    output wire o_data;
    wire[2:0] inv_data;
    
    not not1(inv_data[0], i_data[0]);
    not not2(inv_data[1], i_data[1]);
    not not3(inv_data[2], i_data[2]);
    and_3 and1(i_data[0], inv_data[1], inv_data[2], o_data);
endmodule

module check_2(i_data, o_data);
    input wire[2:0] i_data;
    output wire o_data;
    wire[2:0] inv_data;
    
    not not1(inv_data[0], i_data[0]);
    not not2(inv_data[1], i_data[1]);
    not not3(inv_data[2], i_data[2]);
    and_3 and1(inv_data[0], i_data[1], inv_data[2], o_data);
endmodule

module check_3(i_data, o_data);
    input wire[2:0] i_data;
    output wire o_data;
    wire[2:0] inv_data;
    
    not not1(inv_data[0], i_data[0]);
    not not2(inv_data[1], i_data[1]);
    not not3(inv_data[2], i_data[2]);
    and_3 and1(i_data[0], i_data[1], inv_data[2], o_data);
endmodule

module check_4(i_data, o_data);
    input wire[2:0] i_data;
    output wire o_data;
    wire[2:0] inv_data;
    
    not not1(inv_data[0], i_data[0]);
    not not2(inv_data[1], i_data[1]);
    not not3(inv_data[2], i_data[2]);
    and_3 and1(inv_data[0], inv_data[1], i_data[2], o_data);
endmodule

module rs_mod(r, s, reset, q, inv_q);
    input wire r, s, reset;
    output wire q, inv_q;
    wire res;

    or or1(q, reset, res);
    nor nor1(inv_q, q, s);
    nor nor2(res, r, inv_q);
endmodule

module synch_rs_mod(r, s, clk, reset, q, inv_q);
    input wire r, s, clk, reset;
    output wire q, inv_q;
    wire new_r, new_s;

    and and1(new_r, r, clk);
    and and2(new_s, s, clk);
    rs_mod rs_mod1(new_r, new_s, reset, q, inv_q);
endmodule

module jk_trigger(k, j, clk, reset, q, inv_q);
    input wire k, j, clk, reset;
    output wire q, inv_q;
    wire new_k, new_j, temp_q, temp_inv_q, inv_clk;

    not not1(inv_clk, clk);
    and and1(new_k, k, q);
    and and2(new_j, j, inv_q);
    synch_rs_mod rs1(new_k, new_j, clk, reset, temp_q, temp_inv_q);
    synch_rs_mod rs2(temp_inv_q, temp_q, inv_clk, reset, q, inv_q);
endmodule

module indicator(clk, reset, o_data);
    input wire clk, reset;
    output wire[2:0] o_data;
    supply1 pwr;
    wire t0, t1, t2, reset2, n, inv_data;

    not not1(inv_data, o_data[1]);
    or or1(reset2, reset, n);
    jk_trigger j1(pwr, pwr, clk, reset2, t0, o_data[0]);
    jk_trigger j2(pwr, pwr, o_data[0], reset2, t1, o_data[1]);
    jk_trigger j3(pwr, pwr, o_data[1], reset2, t2, o_data[2]);
    and_3 and1(o_data[0], inv_data, o_data[2], n);
endmodule

module my_checker(a0, a1, o1, res);
    input wire a0, a1, o1;
    output wire res;
    wire temp;

    and and1(temp, a0, a1);
    or or1(res, temp, o1);
endmodule

module conv(clk, a0, a1, a2, o_data);
    input wire clk, a0, a1, a2;
    output wire[2:0] o_data;

    and and1(o_data[0], clk, a0);
    and and2(o_data[1], clk, a1);
    and and3(o_data[2], clk, a2);
endmodule

module or_5(a0, a1, a2, a3, a4, res);
    input wire a0, a1, a2, a3, a4;
    output wire res;
    wire r0, r1, r2;

    or or1(r0, a0, a1);
    or or2(r1, r0, a2);
    or or3(r2, r1, a3);
    or or4(res, r2, a4);
endmodule

module converter_minus(i_data, o_data);
    input wire[2:0] i_data;
    output wire[2:0] o_data;
    wire r0, r1, r2, r3, r4;
    wire[2:0] p0, p1, p2, p3, p4;
    supply1 pwr;
    supply0 grd;

    check_1 ch1(i_data, r0);
    check_2 ch2(i_data, r1);
    check_3 ch3(i_data, r2);
    check_4 ch4(i_data, r3);
    check_0 ch5(i_data, r4);
    conv conv1(r0, grd, grd, grd, p0);
    conv conv2(r1, pwr, grd, grd, p1);
    conv conv3(r2, grd, pwr, grd, p2);
    conv conv4(r3, pwr, pwr, grd, p3);
    conv conv5(r4, grd, grd, pwr, p4);
    or_5 or_5_1(p0[0], p1[0], p2[0], p3[0], p4[0], o_data[0]);
    or_5 or_5_2(p0[1], p1[1], p2[1], p3[1], p4[1], o_data[1]);
    or_5 or_5_3(p0[2], p1[2], p2[2], p3[2], p4[2], o_data[2]);
endmodule

module module_5(i_data, o_data);
    input wire[2:0] i_data;
    output wire[2:0] o_data;
    wire r0, r1, r2, r3, r4, r5, r6, r7;
    wire[2:0] p0, p1, p2, p3, p4;
    wire new_r0, new_r1, new_r2, inv_d0, inv_d1;
    supply1 pwr;
    supply0 grd;

    not not1(inv_d0, i_data[0]);
    not not2(inv_d1, i_data[1]);
    check_0 ch1(i_data, r0);
    check_1 ch2(i_data, r1);
    check_2 ch3(i_data, r2);
    check_3 ch4(i_data, r3);
    check_4 ch5(i_data, r4);
    and_3 and1(inv_d0, i_data[1], i_data[2], r5);
    and_3 and2(i_data[0], inv_d1, i_data[2], r6);
    and_3 and3(i_data[0], i_data[1], i_data[2], r7);
    or or1(new_r0, r0, r5);
    or or2(new_r1, r1, r6);
    or or3(new_r2, r2, r7);
    conv conv1(new_r0, grd, grd, grd, p0);
    conv conv2(new_r1, pwr, grd, grd, p1);
    conv conv3(new_r2, grd, pwr, grd, p2);
    conv conv4(r3, pwr, pwr, grd, p3);
    conv conv5(r4, grd, grd, pwr, p4);
    or_5 or_5_1(p0[0], p1[0], p2[0], p3[0], p4[0], o_data[0]);
    or_5 or_5_2(p0[1], p1[1], p2[1], p3[1], p4[1], o_data[1]);
    or_5 or_5_3(p0[2], p1[2], p2[2], p3[2], p4[2], o_data[2]);
endmodule

module parser(clk, a0, a1, a2, a3, a4, o_data);
    input wire clk, a0, a1, a2, a3, a4;
    output wire[4:0] o_data;

    and and1(o_data[0], clk, a0);
    and and2(o_data[1], clk, a1);
    and and3(o_data[2], clk, a2);
    and and4(o_data[3], clk, a3);
    and and5(o_data[4], clk, a4);
endmodule

module converter(elem_1, elem_2, o_data);
    input wire[2:0] elem_1, elem_2;
    output wire[2:0] o_data;
    wire[4:0] val1, val2, r0, r1, r2, r3, r4;
    wire p00, p11, p22, p33, p44;
    supply1 pwr;
    supply0 grd;
    wire[2:0] p0, p1, p2, p3, p4;

    check_0 ch1(elem_1, val1[0]);
    check_1 ch2(elem_1, val1[1]);
    check_2 ch3(elem_1, val1[2]);
    check_3 ch4(elem_1, val1[3]);
    check_4 ch5(elem_1, val1[4]);
    check_0 ch6(elem_2, val2[0]);
    check_1 ch7(elem_2, val2[1]);
    check_2 ch8(elem_2, val2[2]);
    check_3 ch9(elem_2, val2[3]);
    check_4 ch10(elem_2, val2[4]);
    parser par1(val1[0], val2[0], val2[1], val2[2], val2[3], val2[4], r0);
    parser par2(val1[1], val2[0], val2[1], val2[2], val2[3], val2[4], r1);
    parser par3(val1[2], val2[0], val2[1], val2[2], val2[3], val2[4], r2);
    parser par4(val1[3], val2[0], val2[1], val2[2], val2[3], val2[4], r3);
    parser par5(val1[4], val2[0], val2[1], val2[2], val2[3], val2[4], r4);
    or_5 or_5_1(r0[0], r1[1], r2[2], r3[3], r4[4], p00);
    or_5 or_5_2(r0[4], r1[0], r2[1], r3[2], r4[3], p11);
    or_5 or_5_3(r0[3], r1[4], r2[0], r3[1], r4[2], p22);
    or_5 or_5_4(r0[2], r1[3], r2[4], r3[0], r4[1], p33);
    or_5 or_5_5(r0[1], r1[2], r2[3], r3[4], r4[0], p44);
    conv conv1(p00, grd, grd, grd, p0);
    conv conv2(p11, pwr, grd, grd, p1);
    conv conv3(p22, grd, pwr, grd, p2);
    conv conv4(p33, pwr, pwr, grd, p3);
    conv conv5(p44, grd, grd, pwr, p4);
    or_5 or_5_6(p0[0], p1[0], p2[0], p3[0], p4[0], o_data[0]);
    or_5 or_5_7(p0[1], p1[1], p2[1], p3[1], p4[1], o_data[1]);
    or_5 or_5_8(p0[2], p1[2], p2[2], p3[2], p4[2], o_data[2]);
endmodule

module mem_4_bits(i_data, reset, clk, key, o_data);
    input wire reset, clk, key;
    input wire[3:0] i_data;
    output wire[3:0] o_data;
    wire res;
    wire[3:0] tr_data;

    and and1(res, clk, key);
    d_trigger d1(i_data[0], reset, res, o_data[0], tr_data[0]);
    d_trigger d2(i_data[1], reset, res, o_data[1], tr_data[1]);
    d_trigger d3(i_data[2], reset, res, o_data[2], tr_data[2]);
    d_trigger d4(i_data[3], reset, res, o_data[3], tr_data[3]);
endmodule

module stack_structural_normal(io_data, reset, clk, command, index);
    inout wire[3:0] io_data;
    input wire[2:0] index;
    input wire[1:0] command;
    input wire clk, reset;
    wire r, inv_r, cm1;
    wire[3:0] o_data;
    
    not not1(inv_r, r);
    not not2(cm1, command[1]);
    and and1(r, command[0], cm1);
    cmos cmos1(io_data[0], o_data[0], inv_r, r);
    cmos cmos2(io_data[1], o_data[1], inv_r, r);
    cmos cmos3(io_data[2], o_data[2], inv_r, r);
    cmos cmos4(io_data[3], o_data[3], inv_r, r);
    stack_structural stack(o_data, reset, clk, command, index, io_data);
endmodule 

module stack_structural(o_data, reset, clk, command, index, i_data);
    input wire[3:0] i_data;
    input wire[2:0] index;
    input wire[1:0] command;
    input wire clk, reset;
    output wire[3:0] o_data;
    wire[2:0] command2, index2, index3, el1, el2, ind, minus_ind, cm;
    wire[4:0] values, keys, ors, ands, final_keys;
    wire[3:0] p0, p1, p2, p3, p4, p00, p11, p22, p33, p44;

    not not1(cm[0], command[0]);
    not not2(cm[1], command[1]);
    converter convert_ind(minus_ind, index2, index3);
    module_5 m5(index, index2);
    check_0 ch1(index3, values[0]);
    check_1 ch2(index3, values[1]);
    check_2 ch3(index3, values[2]);
    check_3 ch4(index3, values[3]);
    check_4 ch5(index3, values[4]);
    and_3 and_3_1(clk, command[0], cm[1], command2[0]);
    and_3 and_3_2(clk, cm[0], command[1], command2[1]);
    and_3 and_3_3(clk, command[0], command[1], command2[2]);
    indicator i1(command2[0], reset, el1);
    indicator i2(command2[1], reset, el2);
    converter convert1(el1, el2, ind);
    converter_minus convert2(ind, minus_ind);
    check_0 ch6(ind, keys[0]);
    check_1 ch7(ind, keys[1]);
    check_2 ch8(ind, keys[2]);
    check_3 ch9(ind, keys[3]);
    check_4 ch10(ind, keys[4]);
    and and1(ors[0], command2[2], values[0]);
    and and2(ors[1], command2[2], values[1]);
    and and3(ors[2], command2[2], values[2]);
    and and4(ors[3], command2[2], values[3]);
    and and5(ors[4], command2[2], values[4]);
    check_0 ch11(minus_ind, ands[0]);
    check_1 ch12(minus_ind, ands[1]);
    check_2 ch13(minus_ind, ands[2]);
    check_3 ch14(minus_ind, ands[3]);
    check_4 ch15(minus_ind, ands[4]);
    my_checker my_check1(ands[0], command2[1], ors[0], final_keys[0]);
    my_checker my_check2(ands[1], command2[1], ors[1], final_keys[1]);
    my_checker my_check3(ands[2], command2[1], ors[2], final_keys[2]);
    my_checker my_check4(ands[3], command2[1], ors[3], final_keys[3]);
    my_checker my_check5(ands[4], command2[1], ors[4], final_keys[4]);
    mem_4_bits m4_1(i_data, reset, command2[0], keys[0], p0);
    mem_4_bits m4_2(i_data, reset, command2[0], keys[1], p1);
    mem_4_bits m4_3(i_data, reset, command2[0], keys[2], p2);
    mem_4_bits m4_4(i_data, reset, command2[0], keys[3], p3);
    mem_4_bits m4_5(i_data, reset, command2[0], keys[4], p4);
    new_and new_and1(p0, final_keys[0], p00);
    new_and new_and2(p1, final_keys[1], p11);
    new_and new_and3(p2, final_keys[2], p22);
    new_and new_and4(p3, final_keys[3], p33);
    new_and new_and5(p4, final_keys[4], p44);
    result res(p00, p11, p22, p33, p44, o_data);
endmodule
