<?php

require( "config.inc.php" ); 
require( "Database.class.php" ); 

mysql_set_charset('utf8');

$valor_pregunta  = 100;
$valor_encuentro = 200;

$params = $_GET;

if( count( $params ) > 0 )  {
    
    if( $params[ 'key' ] != "tr1v14ubp" ) {
		print "No tiene permisos";
        return;
    }
        
    $db = new Database( DB_SERVER, DB_USER, DB_PASS, DB_DATABASE );
    $db->connect();

    switch( $params[ 'action' ] )
    {
        case 'login':
        
            $teamName = strtolower( $params[ 'teamName' ] );
            $password = $params[ 'password' ];
        
            $query = $db->query( "select count(*) as cantidad ".
                                 "from trivia_equipos eq      ".
                                 "where eq.nombre_equipo = '".
                                 $teamName ."';" );
                                    
            if( $result = $db->fetch_array( $query ) )
            {
                if( $result[ 'cantidad' ] == 0 )
                {
                    echo 'n';
                    break;
                }
                else
                {
                    $query_extra = $db->query( "select e.clave_equipo    ".
                                               "  from trivia_equipos e  ".
                                               " where e.nombre_equipo = '".
                                               $teamName."';" );
                    
                    if( $result_extra = $db->fetch_array( $query_extra ) )
                    {
                        if( $result_extra[ 'clave_equipo' ] != $password )
                        {
                            echo 'n';
                            break;
                        }
                        else
                        {
                            $query_extra_extra = $db->query( "select e.id_equipo,                                       ".
                                                             "       e.nombre_equipo,                                   ".
                                                             "       e.nombre_integrante_1,                             ".
                                                             "       e.dni_integrante_1,                                ".
                                                             "       e.nombre_integrante_2,                             ".
                                                             "       e.dni_integrante_2,                                ".
                                                             "       e.nombre_integrante_3,                             ".
                                                             "       e.dni_integrante_3,                                ".
                                                             "       sum( case when j.correcto = 'n'                    ".
                                                             "                   or j.correcto is null                  ".
                                                             "                 then 0                                   ".
                                                             "                 else ".$valor_pregunta."                 ".
                                                             "                 end )                                    ".
                                                             "       + ( select count(*) as puntaje                     ".
                                                             "             from trivia_encuentros en                    ".
                                                             "             join trivia_equipos eq                       ".
                                                             "               on en.id_equipo = eq.id_equipo             ".
                                                             "            where eq.nombre_equipo = e.nombre_equipo )    ".
                                                             "       * ".$valor_encuentro." as puntaje                  ".
                                                             "  from trivia_equipos e                                   ".
                                                             "  left join trivia_juegos j                               ".
                                                             "    on e.id_equipo = j.id_equipo                          ".
                                                             " group by e.id_equipo,                                    ".
                                                             "          e.nombre_equipo                                 ".
                                                             " order by sum( case when j.correcto = 'n'                 ".
                                                             "                      or j.correcto is null               ".
                                                             "                    then 0                                ".
                                                             "                    else ".$valor_pregunta."              ".
                                                             "                    end )                                 ".
                                                             "          + ( select count(*) as puntaje                  ".
                                                             "                from trivia_encuentros en                 ".
                                                             "                join trivia_equipos eq                    ".
                                                             "                  on en.id_equipo = eq.id_equipo          ".
                                                             "               where eq.nombre_equipo = e.nombre_equipo ) ".
                                                             "          * ". $valor_encuentro." desc;                   " );
                            
                            $teamId  = 0;
                            $score   = 0;
                            $counter = 0;
                            $ranking = 0;
                            while( $result_extra_extra = $db->fetch_array( $query_extra_extra ) )
                            {
                                $counter++;
                                if( strtolower( $result_extra_extra[ 'nombre_equipo' ] ) == $teamName )
                                {
                                    $teamId         = $result_extra_extra[ 'id_equipo' ];
                                    $member1Name    = $result_extra_extra[ 'nombre_integrante_1' ];
                                    $member2Name    = $result_extra_extra[ 'nombre_integrante_2' ];
                                    $member3Name    = $result_extra_extra[ 'nombre_integrante_3' ];
                                    $ranking        = $counter;
                                    $score          = $result_extra_extra[ 'puntaje' ];
                                }
                            }
                            
                            echo $teamId.','.$teamName.','.$member1Name.','.$member2Name.','.$member3Name.','.$score.','.$ranking.','.$counter;
                            
                            break;
                        }
                    }
                }
            }
        
            break;
        
        case 'init':
        
            $teamName = strtolower( $params[ 'teamName' ] );
            $password = $params[ 'password' ];
            
            $member1Name = strtolower( $params[ 'member1Name' ] );
            $member1Dni = $params[ 'member1Dni' ];
            
            $member2Name = strtolower( $params[ 'member2Name' ] );
            $member2Dni = $params[ 'member2Dni' ];
            
            $member3Name = strtolower( $params[ 'member3Name' ] );
            $member3Dni = $params[ 'member3Dni' ];
        
            $query = $db->query( "select count(*) as cantidad ".
                                 "from trivia_equipos eq ".
                                 "where eq.nombre_equipo = '". $teamName ."';" );
                                    
            if( $result = $db->fetch_array( $query ) )
            {
                if( $result[ 'cantidad' ] == 0 )
                {
                    $db->query( "insert into trivia_equipos( nombre_equipo, clave_equipo, nombre_integrante_1, dni_integrante_1, nombre_integrante_2, dni_integrante_2, nombre_integrante_3, dni_integrante_3 ) ".
                                "values ( '".$teamName."', '".$password."', '".$member1Name."', ".$member1Dni.", '".$member2Name."', ".$member2Dni.", '".$member3Name."', ".$member3Dni." );" );
                }
            }
            
            $query = $db->query( "select e.id_equipo,                                       ".
                                 "       e.nombre_equipo,                                   ".
                                 "       e.nombre_integrante_1,                             ".
                                 "       e.dni_integrante_1,                                ".
                                 "       e.nombre_integrante_2,                             ".
                                 "       e.dni_integrante_2,                                ".
                                 "       e.nombre_integrante_3,                             ".
                                 "       e.dni_integrante_3,                                ".
                                 "       sum( case when j.correcto = 'n'                    ".
                                 "                   or j.correcto is null                  ".
                                 "                 then 0                                   ".
                                 "                 else ".$valor_pregunta."                 ".
                                 "                 end )                                    ".
                                 "       + ( select count(*) as puntaje                     ".
                                 "             from trivia_encuentros en                    ".
                                 "             join trivia_equipos eq                       ".
                                 "               on en.id_equipo = eq.id_equipo             ".
                                 "            where eq.nombre_equipo = e.nombre_equipo )    ".
                                 "       * ".$valor_encuentro." as puntaje                  ".
                                 "  from trivia_equipos e                                   ".
                                 "  left join trivia_juegos j                               ".
                                 "    on e.id_equipo = j.id_equipo                          ".
                                 " group by e.id_equipo,                                    ".
                                 "          e.nombre_equipo                                 ".
                                 " order by sum( case when j.correcto = 'n'                 ".
                                 "                      or j.correcto is null               ".
                                 "                    then 0                                ".
                                 "                    else ".$valor_pregunta."              ".
                                 "                    end )                                 ".
                                 "          + ( select count(*) as puntaje                  ".
                                 "                from trivia_encuentros en                 ".
                                 "                join trivia_equipos eq                    ".
                                 "                  on en.id_equipo = eq.id_equipo          ".
                                 "               where eq.nombre_equipo = e.nombre_equipo ) ".
                                 "          * ". $valor_encuentro." desc;                   " );
            
            $teamId  = 0;
            $score   = 0;
            $counter = 0;
            $ranking = 0;
            while( $result = $db->fetch_array( $query ) )
            {
                $counter++;
                if( strtolower( $result[ 'nombre_equipo' ] ) == $teamName )
                {
                    $teamId  = $result[ 'id_equipo' ];
                    $ranking = $counter;
                    $score   = $result[ 'puntaje' ];
                }
            }
            
            echo $teamId.','.$teamName.','.$member1Name.','.$member2Name.','.$member3Name.','.$score.','.$ranking.','.$counter;
            
            break;
        
        case 'zones':
        
            $query = $db->query( "select z.id_zona,     ".
                                 "       z.nombre_zona  ".
                                 "  from trivia_zonas z ".
                                 " order by z.id_zona;  " );
                                    
            while( $result = $db->fetch_array( $query ) )
            {
                echo $result[ 'id_zona' ].',';
                echo $result[ 'nombre_zona' ].',';
            }
            break;
        
        case 'question':
        
            $id_equipo = $params[ 'teamId' ];
        
            $query = $db->query( "select *                                                   ".
                                 "  from trivia_preguntas p                                  ".
                                 " where not exists ( select *                               ".
                                 "                      from trivia_juegos j                 ".
                                 "                      join trivia_equipos e                ".
                                 "                        on j.id_equipo = e.id_equipo       ".
                                 "                     where j.id_categoria = p.id_categoria ".
                                 "                       and j.id_pregunta = p.id_pregunta   ".
                                 "                       and e.id_equipo = ".$id_equipo." )  ".
                                 " order by rand()                                           ".
                                 " limit 1;                                                  " );
            
            if( !$result = $db->fetch_array( $query ) )
            {
                echo 'juego_terminado';
                break;
            }
        
            $query = $db->query( "select preg.id_categoria,                                           ".
                                 "       preg.id_pregunta,                                            ".
                                 "       preg.texto_pregunta,                                         ".
                                 "       r.id_respuesta,                                              ".
                                 "       r.texto_respuesta                                            ".
                                 "  from ( select p.id_categoria,                                     ".
                                 "                p.id_pregunta,                                      ".
                                 "                p.texto_pregunta                                    ".
                                 "           from trivia_preguntas p                                  ".
                                 "          where not exists ( select *                               ".
                                 "                               from trivia_juegos j                 ".
                                 "                               join trivia_equipos e                ".
                                 "                                 on j.id_equipo = e.id_equipo       ".
                                 "                              where j.id_categoria = p.id_categoria ".
                                 "                                and j.id_pregunta = p.id_pregunta   ".
                                 "                                and e.id_equipo = ".$id_equipo." )  ".
                                 "          order by rand()                                           ".
                                 "          limit 1 ) as preg                                         ".
                                 "  join trivia_respuestas r                                          ".
                                 "    on preg.id_categoria = r.id_categoria                           ".
                                 "   and preg.id_pregunta = r.id_pregunta;                            " );
            
            
            
            if( $result = $db->fetch_array( $query ) )
            {
                $categoryId = $result[ 'id_categoria' ];
                echo $categoryId.',';
                
                $query_extra = $db->query( "select c.nombre_categoria ".
                                           " from trivia_categorias c ".
                                           "where c.id_categoria = ".$categoryId.";" );
                
                if( $result_extra = $db->fetch_array( $query_extra ) )
                {
                    echo $result_extra[ 'nombre_categoria' ].',';
                }
                else
                {
                    echo "Error server,";
                }
            
                echo $result[ 'id_pregunta' ].',';
                echo $result[ 'texto_pregunta' ].',';
                echo $result[ 'id_respuesta' ].',';
                echo $result[ 'texto_respuesta' ].',';
            }
            while( $result = $db->fetch_array( $query ) )
            {
                echo $result[ 'id_respuesta' ].',';
                echo $result[ 'texto_respuesta' ].',';
            }
            break;
            
        case 'stats':
                                 
            $query = $db->query( "select e.id_equipo,                               ".
                                 "       e.nombre_equipo,                           ".
                                 "       sum( case when j.correcto = 'n'            ".
                                 "                   or j.correcto is null          ".
                                 "                 then 0                           ".
                                 "                 else ".$valor_pregunta."         ".
                                 "                 end )                            ".
                                 "       + ( select count(*) as puntaje             ".
                                 "             from trivia_encuentros en            ".
                                 "             join trivia_equipos eq               ".
                                 "               on en.id_equipo = eq.id_equipo     ".
                                 "            where eq.id_equipo = e.id_equipo )    ".
                                 "       * ".$valor_encuentro." as puntaje          ".
                                 "  from trivia_equipos e                           ".
                                 "  left join trivia_juegos j                       ".
                                 "    on e.id_equipo = j.id_equipo                  ".
                                 " group by e.id_equipo,                            ".
                                 "          e.nombre_equipo                         ".
                                 " order by sum( case when j.correcto = 'n'         ".
                                 "                      or j.correcto is null       ".
                                 "                    then 0                        ".
                                 "                    else ".$valor_pregunta."      ".
                                 "                    end )                         ".
                                 "          + ( select count(*) as puntaje          ".
                                 "                from trivia_encuentros en         ".
                                 "                join trivia_equipos eq            ".
                                 "                  on en.id_equipo = eq.id_equipo  ".
                                 "               where eq.id_equipo = e.id_equipo ) ".
                                 "          * ". $valor_encuentro." desc;           " );
            
            $teamId  = $params[ 'teamId' ];
            $score   = 0;
            $counter = 0;
            $ranking = 0;
            while( $result = $db->fetch_array( $query ) )
            {
                $counter++;
                if( $teamId == $result[ 'id_equipo' ] )
                {
                    $ranking = $counter;
                    $score   = $result[ 'puntaje' ];
                }
            }
            
            echo $score.','.$ranking.','.$counter;
            
            break;
            
        case 'detection':
        
            $teamId = $params[ 'teamId' ];
            $markerId = $params[ 'markerId' ];
        
            $query = $db->query( "select *                    ".
                                 "  from trivia_encuentros en ".
                                 " where en.id_equipo = ".$teamId.
                                 "   and en.id_marcador = ".$markerId.";" );
        
            $new = 'f';
            if( !$result = $db->fetch_array( $query ) )
            {
                $db->query( "insert into trivia_encuentros ( id_equipo, id_marcador )".
                            "       values ( ".$teamId.", ".$markerId.")" );
                $new = 's';
            }
                                 
            $query = $db->query( "select e.id_equipo,                               ".
                                 "       sum( case when j.correcto = 'n'            ".
                                 "                   or j.correcto is null          ".
                                 "                 then 0                           ".
                                 "                 else ".$valor_pregunta."         ".
                                 "                 end )                            ".
                                 "       + ( select count(*) as puntaje             ".
                                 "             from trivia_encuentros en            ".
                                 "             join trivia_equipos eq               ".
                                 "               on en.id_equipo = eq.id_equipo     ".
                                 "            where eq.id_equipo = e.id_equipo )    ".
                                 "       * ".$valor_encuentro." as puntaje          ".
                                 "  from trivia_equipos e                           ".
                                 "  left join trivia_juegos j                       ".
                                 "    on e.id_equipo = j.id_equipo                  ".
                                 " group by e.id_equipo                             ".
                                 " order by sum( case when j.correcto = 'n'         ".
                                 "                      or j.correcto is null       ".
                                 "                    then 0                        ".
                                 "                    else ".$valor_pregunta."      ".
                                 "                    end )                         ".
                                 "          + ( select count(*) as puntaje          ".
                                 "                from trivia_encuentros en         ".
                                 "                join trivia_equipos eq            ".
                                 "                  on en.id_equipo = eq.id_equipo  ".
                                 "               where eq.id_equipo = e.id_equipo ) ".
                                 "          * ". $valor_encuentro." desc;           " );
            
            $score   = 0;
            $counter = 0;
            $ranking = 0;
            while( $result = $db->fetch_array( $query ) )
            {
                $counter++;
                if( $teamId == $result[ 'id_equipo' ] )
                {
                    $ranking = $counter;
                    $score   = $result[ 'puntaje' ];
                }
            }
            
            echo $new.','.$score.','.$ranking.','.$counter;
            
            break;
            
        case 'answer':
        
            $teamId = $params[ 'teamId' ];
            $categoryId = $params[ 'categoryId' ];
            $questionId = $params[ 'questionId' ];
            $answerId = $params[ 'answerId' ];

            $query = $db->query( "select *                                 ".
                                 "  from trivia_juegos ju                  ".
                                 " where ju.id_equipo = ".$teamId."        ".
                                 "   and ju.id_categoria = ".$categoryId." ".
                                 "   and ju.id_pregunta = ".$questionId."; ");
            
            if( !$result = $db->fetch_array( $query ) )
            {
                $query_extra = $db->query( "select re.correcta                       ".
                                           "  from trivia_respuestas re              ".
                                           " where re.id_categoria = ".$categoryId." ".
                                           "   and re.id_pregunta = ".$questionId."  ".
                                           "   and re.id_respuesta = ".$answerId );
                
                $correct = 'n';
                if( $result_extra = $db->fetch_array( $query_extra ) )
                {
                    $correct = $result_extra[ 'correcta' ];
                }
                
                $query_extra = $db->query( "insert into trivia_juegos ( id_equipo, id_categoria, id_pregunta, correcto )     ".
                                           "       values ( ".$teamId.", ".$categoryId.", ".$questionId.", '".$correct."' ); " );
                           
                                 
                $query = $db->query( "select e.id_equipo,                               ".
                                     "       e.nombre_equipo,                           ".
                                     "       sum( case when j.correcto = 'n'            ".
                                     "                   or j.correcto is null          ".
                                     "                 then 0                           ".
                                     "                 else ".$valor_pregunta."         ".
                                     "                 end )                            ".
                                     "       + ( select count(*) as puntaje             ".
                                     "             from trivia_encuentros en            ".
                                     "             join trivia_equipos eq               ".
                                     "               on en.id_equipo = eq.id_equipo     ".
                                     "            where eq.id_equipo = e.id_equipo )    ".
                                     "       * ".$valor_encuentro." as puntaje          ".
                                     "  from trivia_equipos e                           ".
                                     "  left join trivia_juegos j                       ".
                                     "    on e.id_equipo = j.id_equipo                  ".
                                     " group by e.id_equipo,                            ".
                                     "          e.nombre_equipo                         ".
                                     " order by sum( case when j.correcto = 'n'         ".
                                     "                      or j.correcto is null       ".
                                     "                    then 0                        ".
                                     "                    else ".$valor_pregunta."      ".
                                     "                    end )                         ".
                                     "          + ( select count(*) as puntaje          ".
                                     "                from trivia_encuentros en         ".
                                     "                join trivia_equipos eq            ".
                                     "                  on en.id_equipo = eq.id_equipo  ".
                                     "               where eq.id_equipo = e.id_equipo ) ".
                                     "          * ". $valor_encuentro." desc;           " );
                
                $score   = 0;
                $counter = 0;
                $ranking = 0;
                while( $result_extra = $db->fetch_array( $query_extra ) )
                {
                    $counter++;
                    if( $teamId == $result_extra[ 'id_equipo' ] )
                    {
                        $ranking = $counter;
                        $score   = $result_extra[ 'puntaje' ];
                    }
                }
                
                echo $correct.','.$score.','.$ranking.','.$counter;
            }
            
            break;
        
        default:
        
            break;
    }

    $db->close();
}
?>
