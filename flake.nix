{
  description = "sdp nix develop";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05";
  };
  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.${system}.default = pkgs.mkShell {
        buildInputs = [
          pkgs.cmake
          pkgs.gcc-arm-embedded
        ];

        shellHook = ''
          export PS1="[sdp] $PS1"
        '';
      };
    };
}

