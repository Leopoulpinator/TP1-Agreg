module StringMap = Map.Make (String);;

type couple = string * int;;

let readFile (file : string) : string =
    In_channel.with_open_bin file In_channel.input_all;;

let changement (c : char) : char =
    if ('A' <= c && 'Z' >= c)
    then (char_of_int (int_of_char c + int_of_char 'a' - int_of_char 'A'))
    else if ('a' <= c && 'z' >= c) || ('0' <= c && '9' >= c) then c else ' ';;

let estVide (s : string) : bool =
    String.length s >= 2 ;;

let fonctionInteree ((d : int StringMap.t), (l : string list)) (s : string) : (int StringMap.t * string list) =
    if (StringMap.mem s d)
    then ((d |> StringMap.update s (Option.map (fun x -> x + 1))), l)
    else (d |> StringMap.add s 1, s::l);;

let suppr (d : int StringMap.t) (s : string) : int StringMap.t =
    if (StringMap.mem s d)
    then (d |> StringMap.remove s)
    else d;;

let parcours ((d : int StringMap.t), (mots : couple list)) (s : string) : (int StringMap.t * couple list) =
    if (StringMap.mem s d)
    then (let (n : int) = StringMap.find s d in (d, (s,n)::mots))
    else (d, mots);;

let comparaison (c : couple) (c2 : couple) : int =
    let (a, b) = c in let (a2, b2) = c2 in
    if (b > b2) then 1 else if (b = b2) then 0 else -1;;

let nPremiers ((n : int), (l : couple list)) (c : couple) : (int * couple list) =
    if (n = 0) then (0, l) else (let (a, b) = c in Printf.printf "%s : %d\n" a b; n-1, l@[c]);;

let () =
    if ((Array.length Sys.argv) <> 3)
    then (Printf.printf "\nTo make this program work you need to provide two arguments : a text file and a number n.\nThis program will then compute the n words that appears the most in the text file (except the words in stop_words.txt and the worlds of one letter).\n\n")
    else begin
        let (fichier : string) = Sys.argv.(1) in
        (*let fp2 = open_in "stop_words.txt" in*)
        let (s : string) = readFile fichier in
        (* On remplace les caracteres indesirables par des ' ', et on remplace les majuscules par des minuscules*)
        let (sModif : string) = String.map changement s in
        let (sEpuree : string list) = List.filter estVide (String.split_on_char ' ' sModif) in
        let ((newDico : int StringMap.t), (listeUnique : string list)) = List.fold_left fonctionInteree (StringMap.empty, []) sEpuree in
        let (aSupprimer : string) = readFile "stop_words.txt" in
        let (listeASupprimer : string list) = String.split_on_char ' ' (String.map changement aSupprimer) in
        let (dicoApresSuppression : int StringMap.t) = List.fold_left suppr newDico listeASupprimer in
        let ((_ : int StringMap.t), (mots : couple list))  = List.fold_left parcours (dicoApresSuppression, []) listeUnique in
        let (motsTries : couple list) = List.rev(List.sort comparaison mots) in
        let ((_ : int), (_ : couple list)) = List.fold_left nPremiers (int_of_string (Sys.argv.(2)), []) motsTries in ()
        (*val split_on_char : char -> string -> string list*)
    end;;