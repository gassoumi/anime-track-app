import React, { useState, useEffect } from 'react'
import axios from 'axios';
import { useParams } from "react-router-dom";
const AnimeApi = "/api/anime";

export default function Anime() {
    console.log("anime  component");
    let { id } = useParams();
    const [anime, setAnime] = useState({});
    const [episodes, setEpisodes] = useState([]);
    const [error, setError] = useState(null);
    const [isLoaded, setIsLoaded] = useState(false);

    useEffect(() => {
        const url = `${AnimeApi}/${id}`;
        const getData = async () => {
            try {
                const res = await axios.get(url);
                setAnime(res.data);
                const respEpisode = await axios.get(`${url}/episode/`);
                setEpisodes(respEpisode.data);
                setIsLoaded(true);
            } catch (err) {
                setIsLoaded(true);
                setError(err);
            }
        };
        getData();
    }, [id])

    if (error) {
        return <div>Error: {error.message}</div>;
    } else if (!isLoaded) {
        return <div>Loading...</div>;
    } else {
        return (
            <div>
                <h2> {anime.name} </h2>
                {/* <div className="card">
                    <div className="card-body"> */}
                <div className="table-responsive">
                    <table className="table table-bordered table-striped">
                        <thead className="table-info">
                            <tr>
                                <th>number</th>
                                <th>Name</th>
                                <th>Size</th>
                                <th>Seeders</th>
                                <th>Leechers</th>
                                <th>Site Url</th>
                                <th>Description</th>
                            </tr>
                        </thead>
                        <tbody>
                            {episodes.map(episode =>
                            (
                                <tr key={episode.fileUrl}>
                                    <td> {episode.number} </td>
                                    <td> {episode.fileName} </td>
                                    <td> {episode.fileSize}  </td>
                                    <td> {episode.nbSeeders} </td>
                                    <td> {episode.nbLeechers} </td>
                                    <td> {episode.siteUrl} </td>
                                    <td> {episode.descrLink} </td>
                                </tr>
                            ))}
                        </tbody>
                    </table>
                </div>
                {/* </div>
                </div> */}
            </div>
        );
    }
}
