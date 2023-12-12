library ieee;
use ieee.std_logic_1164.all;

entity Portao is
    port(
        clk : in std_logic;
        btn_start: in std_logic;
        btn_config: in std_logic;
        btn_portao : in std_logic;
        leds_portao : out std_logic_vector(17 downto 0);
        estado_ant: out std_logic_vector(3 downto 0);
    );
end Portao;

architecture behaviorial of Portao is
    type estados is (FECHADO, ABRINDO, PARADO, ABERTO, FECHANDO, CONFIG);
    signal estado: estados;
begin
    process(clk)
    begin
        if (clk'event and clk='1') and btn_start = '1' then
            estado <= FECHADO;
            case estado is
                when FECHADO =>
                    leds_portao <= "111111111111111111";
                    if btn_config = '1' then
                        estado <= CONFIG;
                        estado_ant <= "000";
                    elsif btn_portao = '1' then 
                        estado <= ABRINDO;
                    end if;
                when ABRINDO =>
                    if btn_portao = '1' then
                        estado <= PARADO;
                    elsif leds_portao = "000000000000000000" then
                        estado <= ABERTO;
                    end if;
                    
                when PARADO =>
                    if btn_portao = '1' then 
                        estado <= ABRINDO;
                    end if;
                when ABERTO =>
                        
                when FECHANDO =>
                    
                when CONFIG => 
                    
            end case;
        end if;
    end process;
end behaviorial;