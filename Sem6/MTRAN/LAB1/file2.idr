import Data.Vect

processData : (Int, String) -> Vect 3 Int -> String
processData (id, name) vec = 
    case name of
        "Idris" => "Language match!"
        _       => "Unknown"

main : IO ()
main = do
    let myPair : (Int, String) = (1, "Idris")
    let myVec : Vect 3 Int = [1, 2, 3]
    let myChar : Char = 'A'
    putStrLn (processData myPair myVec)


-- idris file2.idr -o file2 && ./file2