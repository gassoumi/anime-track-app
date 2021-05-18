import React from 'react'
import {
    Switch,
    Route,
    useRouteMatch,
} from "react-router-dom";
import AnimeList from './components/animeList';
import Anime from './components/anime';
import Log from "./components/log";

export default function AppRouter() {
    let match = useRouteMatch();
    console.log("url " + match.path);
    return (
        <Switch>
            <Route path="/anime">
                <Switch>
                    <Route exact path={`/anime/`}>
                        <AnimeList />
                    </Route>
                    <Route exact path={`/anime/:id`}>
                        <Anime />
                    </Route>
                </Switch>
            </Route>
            <Route exact path="/log">
                <Log />
            </Route>
            <Route path="/">
                <AnimeList />
            </Route>
        </Switch>
    )
}
