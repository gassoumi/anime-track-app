import React, { useState, useEffect } from 'react';
import axios from 'axios';
import moment from 'moment';
import AnimeButton from './animeButton';
import ModalDeleteConfirmation from './modalDeleteConfirmation';

import {
    Link
} from "react-router-dom";



const API = "/api/anime/";
function AnimeList() {

    const [animes, setAnimes] = useState([]);
    const [show, setShow] = useState(false);
    const [selectedAnime, setSelectedAnime] = useState({});

    const handleClose = () => {
        setShow(false);
        setSelectedAnime({});
    };

    const handleDeleteAnime = (anime) => {
        setSelectedAnime(anime);
        setShow(true);
    }

    useEffect(() => {
        axios.get(API)
            .then(res => {
                setAnimes(res.data);
            }).catch(err => console.log(err));
    }, [])

    return (
        <div>
            <AnimeButton />
            <ModalDeleteConfirmation
                anime={selectedAnime}
                show={show}
                handleClose={handleClose}
            />
            <div className="row pt-2">
                <div className="col-12">
                    {/* <div className="card">
                        <div className="card-body"> */}
                    <div className="table-responsive">
                        <table className="table table-bordered table-striped">
                            <thead className="table-info">
                                <tr>
                                    <th scope="col">Name</th>
                                    <th scope="col">Search key</th>
                                    <th scope="col">Date added</th>
                                    <th></th>
                                </tr>
                            </thead>
                            <tbody>
                                {
                                    animes.map(anime =>
                                        <tr key={anime.name}>
                                            <td>
                                                <Link to={`/anime/${anime.name}`}>
                                                    {anime.name}
                                                </Link>
                                            </td>
                                            <td> {anime.searchKey} </td>
                                            <td> ({moment(anime.dateAjout).format('LLL')})  </td>
                                            <td>
                                                <button onClick={() => handleDeleteAnime(anime)} type="button" className="btn btn-danger btn-sm mr-1">Delete</button>
                                                <button ctype="button" className="btn btn-sm btn-secondary">Edit</button>
                                            </td>
                                        </tr>
                                    )
                                }
                            </tbody>
                        </table>
                    </div>
                    {/* </div>
                    </div> */}
                </div>
            </div>
        </div>
    )
}


export default AnimeList

