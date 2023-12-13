library ieee;
use ieee.std_logic_1164.all;

entity PROJETO_SD is
    port(
        clk : in std_logic;
        botao : in std_logic;
        switch_vel: in std_logic;
        ea: in std_logic;
        aberto_fechado : in std_logic;
        leds : out std_logic_vector(17 downto 0);
    );
end PROJETO_SD;

architecture behaviorial of PROJETO_SD is
    type estados is (FECHADO_NP,FECHADO_P,FECHANDO_P,FECHANDO_NP,ABRINDO_NP,ABRINDO_P, PARADO_P,PARADO_NP, ABERTO_NP,ABERTO_P, CONFIG_P,CONFIG_NP);
    signal estado: estados;
begin
  process(clk)
  begin 
    if clk'event and clock = '1' then
      case estado is
        when FECHADO_NP =>
          if botao = '1' then
            estado <= ABRINDO_P;
          elsif switch_vel = '1' then
            estado <= CONFIG_NP;
          end if;
        when ABRINDO_P =>
          if leds = "000000000000000000" and botao = '1' then
            estado <= ABERTO_P;
          elsif botao = '0' then
            estado <= ABRINDO_NP;
          elsif botao = '1':
            estado <= ABRINDO_P;
          end if;
        when ABERTO_P =>
          if switch_vel = '1' then
            estado <= CONFIG_P;
          elsif botao = '1' then 
            estado <= ABERTO_P;
          elsif botao = '0' then
            estado <= ABERTO_NP;
          end if;
        when ABERTO_NP => 
          if switch_vel = '1' then
            estado <= CONFIG_NP;
          elsif botao = '1' then
            estado <= FECHANDO_P;
          else then
            estado <= ABERTO_NP;
          end if;
        when FECHANDO_P =>
          if botao = '1' then
            estado <= FECHANDO_P;
          else then
            estado <= FECHANDO_NP;
          end if;
        when FECHADO_NP =>
          ea <= '0';
          if leds = '111111111111111111' then 
            estado <= FECHADO_P;
          elsif botao = '1' then
            estado <= PARADO_P;
          else then 
            estado <= FECHADO_NP;
          end if;
        when PARADO_P =>
          if botao = '1' then
            estado <= PARADO_P;
          else then 
            estado <= PARADO_NP;
          end if;
        when PARADO_NP =>
          if botao = '0' then
            estado <= PARADO_NP;
          elsif botao = '1' and ea = '0' then
            estado <= ABRINDO_NP;
          else then
            estado <= FECHANDO_P;
          end if;
        when CONFIG_P =>
          if switch_vel = '0' and leds = '111111111111111111' then 
            estado <= FECHADO_P;
          elsif botao = '1' then
            estado <= CONFIG_P;
          else then 
            estado <= CONFIG_NP;
          end if;
        when CONFIG_NP =>
          if switch_vel = '0' and leds = '111111111111111111' then 
            estado <= FECHADO_NP;
          elsif switch_vel = '0' and leds = '000000000000000000' then 
            estado <= ABERTO_NP;
          elsif switch_vel = '1' then 
            estado <= CONFIG_NP;
          end if;
      end case;
    end if;
  end process;
end behaviorial;